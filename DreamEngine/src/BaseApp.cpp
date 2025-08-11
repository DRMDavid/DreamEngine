#include "BaseApp.h"
#include "Window.h"
#include "CShape.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "ActorPicker.h"
#include "ActorSerializer.h"
#include "Circuits/Circuit1.h"

BaseApp::~BaseApp() {}

int BaseApp::run() {
    if (!init()) {
        ERROR("BaseApp", "run", "Initialization failed.");
    }
    while (m_windowPtr->isOpen()) {
        m_windowPtr->handleEvents(m_engineGUI);
        ImGui::SFML::Update(*m_windowPtr->m_windowPtr, m_windowPtr->deltaTime);
        update();
        render();
    }
    destroy();
    return 0;
}

bool BaseApp::init() {
    ResourceManager& resourceMan = ResourceManager::getInstance();

    m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "DreamEngine");
    if (m_windowPtr.isNull()) {
        ERROR("BaseApp", "init", "Failed to create window pointer.");
        return false;
    }

    m_engineGUI.init(m_windowPtr);

    m_ATrack = EngineUtilities::MakeShared<Actor>("Track Actor");
    if (m_ATrack) {
        m_ATrack->getComponent<CShape>()->createShape(RECTANGLE);
        m_ATrack->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_ATrack->getComponent<Transform>()->setPosition(sf::Vector2f(450.f, 0.f));
        m_ATrack->getComponent<Transform>()->setScale(sf::Vector2f(10.f, 19.5f));
        if (!resourceMan.loadTexture("sprites/Pista", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_ATrack->setTexture(resourceMan.getTexture("sprites/Pista"));
    }

    m_player = EngineUtilities::MakeShared<A_Player>("Player");
    if (m_player) {
        m_player->getComponent<CShape>()->createShape(CIRCLE);
        m_player->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_player->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));
        m_player->setPosition(sf::Vector2f(880.f, 880.f));
        m_player->setControlMode(PlayerControlMode::Direct);
        m_player->setAcceleration(700.f);
        m_player->setFriction(6.f);
        m_player->setMaxSpeed(m_sharedMaxSpeed);
        if (!resourceMan.loadTexture("sprites/Yoshi", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_player->setTexture(resourceMan.getTexture("sprites/Yoshi"));
    }

    m_racerNPC = EngineUtilities::MakeShared<A_Racer>("NPC_1");
    if (m_racerNPC) {
        m_racerNPC->getComponent<CShape>()->createShape(CIRCLE);
        m_racerNPC->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_racerNPC->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));
        if (!resourceMan.loadTexture("sprites/Fantasma", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_racerNPC->setTexture(resourceMan.getTexture("sprites/Fantasma"));
        m_racerNPC->setSpeed(m_sharedMaxSpeed);
        m_racerNPC->setArriveRadius(40.f);
        m_racerNPC->setMode(SteeringMode::Arrive);
        m_racerNPC->enableSteering(false);
        m_racerNPC->setPosition(sf::Vector2f(855.f, 855.f));
    }
    m_npcs.clear();
    if (!m_racerNPC.isNull()) m_npcs.push_back(m_racerNPC);

    m_waypoints = getCircuitWaypoints();

    {
        PlayerInputConfig pic;
        pic.player = m_player;
        pic.renderWindow = m_windowPtr->m_windowPtr.get();
        m_playerInputSystem = EngineUtilities::MakeUnique<PlayerInputSystem>(pic);
    }

    {
        WaypointFollowConfig wfc;
        wfc.racers = m_npcs;
        wfc.waypoints = &m_waypoints;
        wfc.arriveRadiusForAdvance = 22.f;
        wfc.reactionDelay = 0.14f;
        wfc.waypointNoiseRadius = 6.f;
        wfc.lookAhead = 1;
        wfc.mistakeProb = 0.03f;
        wfc.mistakeCooldown = 2.0f;
        wfc.corridorWidth = 80.f;
        wfc.cornerSlowdownEnabled = true;
        wfc.cornerMinAngleDeg = 25.f;
        wfc.cornerMaxAngleDeg = 95.f;
        wfc.cornerMinFactor = 0.82f;
        m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
        m_waypointFollowSystem->primeFromPositions();
    }

    {
        SteeringConfig sc;
        sc.racers = m_npcs;
        sc.seekRange = 10.f;
        m_steeringSystem = EngineUtilities::MakeUnique<SteeringSystem>(sc);
    }

    {
        RaceConfig rc;
        rc.actors.clear();
        if (!m_player.isNull())   rc.actors.push_back(m_player);
        for (auto& npc : m_npcs)  rc.actors.push_back(npc);
        rc.waypoints = &m_waypoints;
        rc.checkpointRadius = 12.f;
        m_raceSystem = EngineUtilities::MakeUnique<RaceSystem>(rc);
    }

    m_countdown.start();
    m_raceArmed = true;
    m_raceLive = false;
    m_raceFinished = false;
    m_finalPlace = -1;
    m_npcFinished = false;

    return true;
}

void BaseApp::update() {
    if (!m_windowPtr.isNull()) {
        m_windowPtr->update();
    }
    const float dt = m_windowPtr->deltaTime.asSeconds();

    std::string cdText;
    if (m_raceArmed && !m_raceLive) {
        cdText = m_countdown.tick(dt);
        if (m_countdown.isFinished()) {
            m_raceLive = true;
            m_raceArmed = false;
            if (!m_racerNPC.isNull()) {
                m_racerNPC->enableSteering(true);
            }
            if (!m_raceSystem.isNull()) {
                m_raceSystem->setTimingActive(true);
            }
        }
    }

    if (!m_player.isNull()) {
        float playerMax = m_player->getMaxSpeed();
        if (playerMax <= 0.f) playerMax = m_sharedMaxSpeed;
        m_player->setMaxSpeed(playerMax);

        if (!m_racerNPC.isNull()) {
            float npcMax = playerMax * m_npcSpeedFactor;
            m_racerNPC->setSpeed(npcMax);
        }
    }

    if (m_raceLive && !m_raceFinished) {
        if (!m_playerInputSystem.isNull()) { m_playerInputSystem->update(dt); }
        if (!m_waypointFollowSystem.isNull()) { m_waypointFollowSystem->update(dt); }
        if (!m_steeringSystem.isNull()) { m_steeringSystem->update(dt); }
    }

    if (!m_raceSystem.isNull()) { m_raceSystem->update(dt); }

    if (!m_raceFinished && !m_raceSystem.isNull()) {
        int playerLap = m_raceSystem->getLapData(0).lap;
        int npcLap = (m_raceSystem->getLapData(1).lap);
        if (!m_npcFinished && npcLap >= m_lapsToWin) {
            m_npcFinished = true;
            if (!m_racerNPC.isNull()) {
                m_racerNPC->enableSteering(false);
                m_racerNPC->setSpeed(0.f);
            }
        }
        if (playerLap >= m_lapsToWin) {
            m_raceFinished = true;
            m_raceLive = false;
            std::vector<int> order = m_raceSystem->getStandings();
            m_finalPlace = -1;
            for (size_t k = 0; k < order.size(); ++k) {
                if (order[k] == 0) {
                    m_finalPlace = static_cast<int>(k + 1);
                    break;
                }
            }
        }
    }

    if (!m_ATrack.isNull())    m_ATrack->update(dt);
    if (!m_player.isNull())    m_player->update(dt);
    if (!m_racerNPC.isNull())  m_racerNPC->update(dt);

    actorsVector.clear();
    if (!m_ATrack.isNull())    actorsVector.push_back(m_ATrack);
    if (!m_player.isNull())    actorsVector.push_back(m_player);
    if (!m_racerNPC.isNull())  actorsVector.push_back(m_racerNPC);

    m_engineGUI.menuBar();
    m_engineGUI.hierarchy(actorsVector);
    m_engineGUI.inspector(actorsVector);
    m_engineGUI.console();
    m_engineGUI.fileManagerPanel(actorsVector);

    if (!ImGui::GetIO().WantCaptureMouse && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        int pickedIdx = ActorPicker::pickActorUnderMouse(m_windowPtr->m_windowPtr.get(), actorsVector);
        if (pickedIdx != -1) {
            m_engineGUI.selectedActorIndex = pickedIdx;
        }
    }

    if (!m_raceLive && !m_raceFinished && !cdText.empty()) {
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(
            ImVec2((float)m_windowPtr->m_windowPtr->getSize().x * 0.5f,
                (float)m_windowPtr->m_windowPtr->getSize().y * 0.35f),
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("##countdown_overlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%s", cdText.c_str());
        ImGui::End();
    }

    if (!m_raceSystem.isNull() && !m_raceFinished) {
        std::vector<int> order = m_raceSystem->getStandings();
        int place = -1;
        for (size_t k = 0; k < order.size(); ++k) {
            if (order[k] == 0) {
                place = static_cast<int>(k + 1);
                break;
            }
        }
        int lapHUD_real = m_raceSystem->getLapData(0).lap;
        int lapHUD = lapHUD_real + 1;
        auto fmt = [](float sec) -> std::string {
            if (sec < 0.f) return std::string("--:--.--");
            int total_ms = static_cast<int>(sec * 1000.f + 0.5f);
            int minutes = total_ms / 60000;
            int seconds = (total_ms / 1000) % 60;
            int hundred = (total_ms % 1000) / 10;
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%02d:%02d.%02d", minutes, seconds, hundred);
            return std::string(buf);
            };
        float bestLap = m_raceSystem->getPlayerBestLapTime();
        float currLap = m_raceSystem->getPlayerCurrentLapTime();
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::SetNextWindowPos(ImVec2(40.f, 40.f), ImGuiCond_Always);
        ImGui::Begin("##race_hud_lappos", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("Lap: %d / %d", lapHUD, m_lapsToWin);
        if (place > 0) ImGui::Text("Position: %d", place);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Text("Best:    %s", fmt(bestLap).c_str());
        ImGui::Text("Current: %s", fmt(currLap).c_str());
        ImGui::End();
    }

    if (m_raceFinished) {
        ImGui::SetNextWindowBgAlpha(0.2f);
        ImGui::SetNextWindowPos(
            ImVec2((float)m_windowPtr->m_windowPtr->getSize().x * 0.5f,
                (float)m_windowPtr->m_windowPtr->getSize().y * 0.5f),
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("##finish_overlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowFontScale(3.0f);
        if (m_finalPlace == 1) {
            ImGui::Text("¡Ganaste! 1ro");
        }
        else if (m_finalPlace > 0) {
            ImGui::Text("Terminaste %do", m_finalPlace);
        }
        else {
            ImGui::Text("Carrera terminada");
        }
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Spacing();
        if (ImGui::Button("Reintentar", ImVec2(240.f, 60.f))) {
            resetRace();
        }
        ImGui::End();
    }
}

void BaseApp::render() {
    if (m_windowPtr.isNull()) return;
    m_windowPtr->clear();
    if (m_ATrack)    m_ATrack->getComponent<CShape>()->render(m_windowPtr);
    if (m_player)    m_player->getComponent<CShape>()->render(m_windowPtr);
    if (m_racerNPC)  m_racerNPC->getComponent<CShape>()->render(m_windowPtr);
    m_engineGUI.drawSelectedOutline(m_windowPtr->m_windowPtr.get(), actorsVector);
    m_windowPtr->render();
    m_engineGUI.render(m_windowPtr);
    m_windowPtr->display();
}

void BaseApp::destroy() {
    m_engineGUI.destroy();
}

void BaseApp::resetRace() {
    if (!m_player.isNull()) {
        m_player->setPosition(sf::Vector2f(880.f, 880.f));
    }
    if (!m_racerNPC.isNull()) {
        m_racerNPC->setPosition(sf::Vector2f(855.f, 855.f));
        m_racerNPC->enableSteering(false);
    }
    {
        WaypointFollowConfig wfc;
        wfc.racers = m_npcs;
        wfc.waypoints = &m_waypoints;
        wfc.arriveRadiusForAdvance = 22.f;
        wfc.reactionDelay = 0.14f;
        wfc.waypointNoiseRadius = 6.f;
        wfc.lookAhead = 1;
        wfc.mistakeProb = 0.03f;
        wfc.mistakeCooldown = 2.0f;
        wfc.corridorWidth = 80.f;
        wfc.cornerSlowdownEnabled = true;
        wfc.cornerMinAngleDeg = 25.f;
        wfc.cornerMaxAngleDeg = 95.f;
        wfc.cornerMinFactor = 0.82f;
        m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
        m_waypointFollowSystem->primeFromPositions();
    }
    {
        RaceConfig rc;
        rc.actors.clear();
        if (!m_player.isNull())   rc.actors.push_back(m_player);
        for (auto& npc : m_npcs)  rc.actors.push_back(npc);
        rc.waypoints = &m_waypoints;
        rc.checkpointRadius = 12.f;
        m_raceSystem = EngineUtilities::MakeUnique<RaceSystem>(rc);
    }
    m_countdown = Countdown{ 3.f };
    m_countdown.start();
    m_raceArmed = true;
    m_raceLive = false;
    m_raceFinished = false;
    m_finalPlace = -1;
    m_npcFinished = false;
    if (!m_raceSystem.isNull()) {
        m_raceSystem->setTimingActive(false);
    }
}
