/**
 * @file BaseApp.cpp
 * @brief Implementación principal de la clase BaseApp.
 * @details Gestiona el ciclo de vida de la aplicación, la creación de actores,
 *  la configuración de sistemas (entrada, seguimiento de waypoints, steering,
 *  cronometraje de carrera) y el HUD con ImGui.
 */

#include "BaseApp.h"
#include "Window.h"
#include "CShape.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include "ActorPicker.h"
#include "ActorSerializer.h"
#include "Circuits/Circuit1.h"
#include <imgui.h>

 /**
  * @brief Destructor por defecto de BaseApp.
  */
BaseApp::~BaseApp() {}

/**
 * @brief Ejecuta el bucle principal de la aplicación.
 * @return 0 si la ejecución termina correctamente.
 * @details
 *  - Llama a @ref BaseApp::init para inicializar subsistemas.
 *  - Mientras la ventana esté abierta:
 *    - Procesa eventos (incluida la GUI).
 *    - Actualiza la lógica del juego con @ref BaseApp::update.
 *    - Dibuja la escena con @ref BaseApp::render.
 *  - Al final, llama a @ref BaseApp::destroy.
 */
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

/**
 * @brief Inicializa ventana, GUI, actores, waypoints y sistemas.
 * @return true si la inicialización fue exitosa; false en caso de error.
 * @details
 *  - Crea la ventana y arranca la GUI.
 *  - Crea los actores (pista, jugador, NPC) y les asigna forma/escala/textura.
 *  - Carga el circuito de waypoints.
 *  - Configura los sistemas de entrada, seguimiento de waypoints, steering y
 *    carrera.
 *  - Inicializa el estado de la carrera (cuenta atrás y banderas).
 */
bool BaseApp::init() {
  ResourceManager& resourceMan = ResourceManager::getInstance();

  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "DreamEngine");
  if (m_windowPtr.isNull()) {
    ERROR("BaseApp", "init", "Failed to create window pointer.");
    return false;
  }

  m_engineGUI.init(m_windowPtr);

  // ====== TRACK ======
  /**
   * @brief Configuración del actor de pista (sprite/shape de fondo).
   * @details Se crea un rectángulo escalado y se le asigna una textura de pista.
   */
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

  // ====== PLAYER ======
  /**
   * @brief Configuración del jugador.
   * @details Se crea con forma circular, se posiciona sobre la meta y se
   *  parametriza su control directo (WASD), aceleración, fricción y velocidad
   *  máxima. Se asigna textura.
   */
  m_player = EngineUtilities::MakeShared<A_Player>("Player");
  if (m_player) {
    m_player->getComponent<CShape>()->createShape(CIRCLE);
    m_player->getComponent<CShape>()->setFillColor(sf::Color::White);
    m_player->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));
    // Spawn en la meta nueva
    m_player->setPosition(sf::Vector2f(1329.f, 249.f));
    m_player->setControlMode(PlayerControlMode::Direct);
    m_player->setAcceleration(700.f);
    m_player->setFriction(6.f);
    m_player->setMaxSpeed(m_sharedMaxSpeed);
    if (!resourceMan.loadTexture("sprites/Yoshi", "png")) {
      MESSAGE("BaseApp", "Init", "Can't load texture.");
    }
    m_player->setTexture(resourceMan.getTexture("sprites/Yoshi"));
  }

  // ====== NPC ======
  /**
   * @brief Configuración del NPC principal.
   * @details NPC con forma circular, textura, velocidad y modo Arrive. Se
   *  sitúa junto al jugador. El steering se habilita al iniciar la carrera.
   */
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
    m_racerNPC->setArriveRadius(60.f);    // llegada más suave al target
    m_racerNPC->setMode(SteeringMode::Arrive);
    // Spawn junto al player
    m_racerNPC->setPosition(sf::Vector2f(1329.f, 249.f));
    m_racerNPC->enableSteering(false);    // se habilita cuando arranca la carrera
  }
  m_npcs.clear();
  if (!m_racerNPC.isNull()) m_npcs.push_back(m_racerNPC);

  // ====== WAYPOINTS ======
  /**
   * @brief Carga de waypoints del circuito.
   * @details Se registran para los sistemas de seguimiento/race y se escribe
   *  un pequeño log con la cantidad y extremos.
   */
  m_waypoints = getCircuitWaypoints();
  // Log de verificación
  {
    const auto n = m_waypoints.size();
    const auto f = m_waypoints.empty() ? sf::Vector2f(0.f, 0.f) : m_waypoints.front();
    const auto l = m_waypoints.empty() ? sf::Vector2f(0.f, 0.f) : m_waypoints.back();
    MESSAGE("BaseApp", "init",
      ("WP count: " + std::to_string(n) +
        " first: (" + std::to_string(f.x) + "," + std::to_string(f.y) + ")" +
        " last: (" + std::to_string(l.x) + "," + std::to_string(l.y) + ")").c_str());
  }

  // ====== SISTEMAS ======
  /**
   * @brief Sistema de entrada del jugador (teclado/ratón).
   */
  {
    PlayerInputConfig pic;
    pic.player = m_player;
    pic.renderWindow = m_windowPtr->m_windowPtr.get();
    m_playerInputSystem = EngineUtilities::MakeUnique<PlayerInputSystem>(pic);
  }

  /**
   * @brief Sistema de seguimiento de waypoints (IA de ruta para NPCs).
   * @details Configurado en modo estricto (sin jitter ni errores) para evitar
   *  atajos y forzar paso correcto por los puntos.
   */
  {
    WaypointFollowConfig wfc;
    wfc.racers = m_npcs;
    wfc.waypoints = &m_waypoints;

    // Más estrictos para evitar "saltos" y atajos
    wfc.arriveRadiusForAdvance = 8.f;   // exige estar más cerca para avanzar
    wfc.corridorWidth = 24.f;  // corredor estrecho (reduce crossedForward)
    wfc.lookAhead = 0;     // sin mirar por delante
    wfc.waypointNoiseRadius = 0.f;   // sin jitter
    wfc.mistakeProb = 0.f;   // sin errores adrede
    wfc.reactionDelay = 0.14f;

    wfc.cornerSlowdownEnabled = true;
    wfc.cornerMinAngleDeg = 25.f;
    wfc.cornerMaxAngleDeg = 95.f;
    wfc.cornerMinFactor = 0.78f; // un poco más de freno en curva

    m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
    m_waypointFollowSystem->primeFromPositions();
    m_waypointFollowSystem->update(0.016f);
  }

  /**
   * @brief Sistema de steering (Seek/Arrive/Pursuit).
   */
  {
    SteeringConfig sc;
    sc.racers = m_npcs;
    sc.seekRange = 10.f;
    m_steeringSystem = EngineUtilities::MakeUnique<SteeringSystem>(sc);
  }

  /**
   * @brief Sistema de carrera (vueltas, tiempos, checkpoints, standings).
   */
  {
    RaceConfig rc;
    rc.actors.clear();
    if (!m_player.isNull())   rc.actors.push_back(m_player);
    for (auto& npc : m_npcs)  rc.actors.push_back(npc);
    rc.waypoints = &m_waypoints;
    rc.checkpointRadius = 12.f;
    m_raceSystem = EngineUtilities::MakeUnique<RaceSystem>(rc);
  }

  // Estado inicial
  /**
   * @brief Activa la cuenta atrás de arranque y valores iniciales de estado
   *  de la carrera y banderas de control.
   */
  m_countdown.start();
  m_raceArmed = true;
  m_raceLive = false;
  m_raceFinished = false;
  m_finalPlace = -1;
  m_npcFinished = false;

  return true;
}

/**
 * @brief Actualiza la lógica por frame.
 * @details
 *  - Actualiza deltaTime de la ventana.
 *  - Gestiona la cuenta atrás y arranque de carrera (arma contador de vueltas,
 *    asigna owner, activa timing, habilita steering del NPC).
 *  - Ajusta velocidades (jugador/NPC).
 *  - Ejecuta sistemas (entrada, seguimiento, steering, carrera).
 *  - Comprueba fin de carrera y calcula posición final.
 *  - Dibuja overlays/HUD con ImGui (cuenta atrás, datos de carrera, fin).
 *  - Actualiza actores y paneles de la GUI (jerarquía, inspector, etc.).
 * @note No modifica la lógica de renderizado; solo prepara estado.
 */
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

      // === ARRANQUE DE CARRERA: armar contador, definir owner, timing y steering ===
      if (!m_raceSystem.isNull()) {
        m_raceSystem->setLapOwnerIndex(0); // player es índice 0
        m_raceSystem->armLapCounter(true); // arma para contar en la primera pasada
        m_raceSystem->setTimingActive(true);
      }
      if (!m_racerNPC.isNull()) {
        m_racerNPC->enableSteering(true);
      }
    }
  }

  // Ajuste dinámico de velocidades
  if (!m_player.isNull()) {
    float playerMax = m_player->getMaxSpeed();
    if (playerMax <= 0.f) playerMax = m_sharedMaxSpeed;
    m_player->setMaxSpeed(playerMax);

    if (!m_racerNPC.isNull()) {
      float npcMax = playerMax * m_npcSpeedFactor;
      m_racerNPC->setSpeed(npcMax);
    }
  }

  // PlayerInput solo en live; follow & steering siempre (para asegurar actualización)
  if (m_raceLive && !m_raceFinished) {
    if (!m_playerInputSystem.isNull()) { m_playerInputSystem->update(dt); }
  }
  if (!m_waypointFollowSystem.isNull()) { m_waypointFollowSystem->update(dt); }
  if (!m_steeringSystem.isNull()) { m_steeringSystem->update(dt); }

  if (!m_raceSystem.isNull()) { m_raceSystem->update(dt); }

  // === Fin de carrera / posición ===
  if (!m_raceFinished && !m_raceSystem.isNull()) {
    int playerLap = m_raceSystem->getLapData(0).lap;
    int npcLap = m_raceSystem->getLapData(1).lap;

    // Si el NPC llegara a 3 (no suele contar si owner=0), lo detenemos
    if (!m_npcFinished && npcLap >= m_lapsToWin) {
      m_npcFinished = true;
      if (!m_racerNPC.isNull()) {
        m_racerNPC->enableSteering(false);
        m_racerNPC->setSpeed(0.f);
      }
    }

    // Gana el player al alcanzar el número de vueltas
    if (playerLap >= m_lapsToWin) {
      m_raceFinished = true;
      m_raceLive = false;
      std::vector<int> order = m_raceSystem->getStandings();
      m_finalPlace = -1;
      for (size_t k = 0; k < order.size(); ++k) {
        if (order[k] == 0) { m_finalPlace = (int)k + 1; break; }
      }
    }
  }

  // === HUDs (cuenta atrás / vueltas-pos / tiempos / fin) ===

  // Cuenta atrás superpuesta
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

  // Panel de vueltas / posición / tiempos (mientras la carrera vive)
  if (!m_raceSystem.isNull() && !m_raceFinished) {
    std::vector<int> order = m_raceSystem->getStandings();
    int place = -1;
    for (size_t k = 0; k < order.size(); ++k) {
      if (order[k] == 0) { place = (int)k + 1; break; }
    }
    int lapHUD_real = m_raceSystem->getLapData(0).lap;
    int lapHUD = lapHUD_real + 1;

    /**
     * @brief Lambda local para formatear tiempos en mm:ss.hh
     * @param sec Segundos en punto flotante.
     * @return Cadena con el formato mm:ss.hh o "--:--.--" si sec < 0.
     */
    auto fmt = [](float sec) -> std::string {
      if (sec < 0.f) return std::string("--:--.--");
      int total_ms = (int)(sec * 1000.f + 0.5f);
      int minutes = total_ms / 60000;
      int seconds = (total_ms / 1000) % 60;
      int hundred = (total_ms % 1000) / 10;
      char buf[32];
      std::snprintf(buf, sizeof(buf), "%02d:%02d.%02d", minutes, seconds, hundred);
      return std::string(buf);
      };
    float bestLap = m_raceSystem->getPlayerBestLapTime();
    float currLap = m_raceSystem->getPlayerCurrentLapTime();

    ImGui::SetNextWindowBgAlpha(0.30f);
    ImGui::SetNextWindowPos(ImVec2(40.f, 40.f), ImGuiCond_Always);
    ImGui::Begin("##race_hud_lappos", nullptr,
      ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_NoInputs |
      ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SetWindowFontScale(2.0f);

    // Colores (ajusta a tu gusto)
    const ImVec4 colLap = ImVec4(1.00f, 0.85f, 0.20f, 1.00f); // dorado
    const ImVec4 colPosition = ImVec4(0.50f, 0.80f, 1.00f, 1.00f); // celeste
    const ImVec4 colBest = ImVec4(0.70f, 1.00f, 0.70f, 1.00f); // verde suave
    const ImVec4 colCurrent = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // blanco

    ImGui::TextColored(colLap, "Lap: %d / %d", lapHUD, m_lapsToWin);
    if (place > 0) ImGui::TextColored(colPosition, "Position: %d", place);

    ImGui::SetWindowFontScale(1.0f);
    ImGui::Separator();
    ImGui::TextColored(colBest, "Best:    %s", fmt(bestLap).c_str());
    ImGui::TextColored(colCurrent, "Current: %s", fmt(currLap).c_str());

    ImGui::End();
  }

  // Overlay de fin de carrera
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

    const ImVec4 colWin = ImVec4(0.20f, 0.95f, 0.35f, 1.00f); // verde
    const ImVec4 colPlace = ImVec4(1.00f, 0.85f, 0.25f, 1.00f); // dorado
    const ImVec4 colEnd = ImVec4(1.00f, 0.40f, 0.40f, 1.00f); // rojizo

    if (m_finalPlace == 1) {
      ImGui::TextColored(colWin, u8"¡Ganaste! 1ro");
    }
    else if (m_finalPlace > 0) {
      ImGui::TextColored(colPlace, "Terminaste %do", m_finalPlace);
    }
    else {
      ImGui::TextColored(colEnd, "Carrera terminada");
    }

    ImGui::SetWindowFontScale(1.2f);
    ImGui::Spacing();
    if (ImGui::Button("Reintentar", ImVec2(240.f, 60.f))) {
      resetRace();
    }
    ImGui::End();
  }

  // === Actores y GUI lateral ===
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
}

/**
 * @brief Dibuja la escena y los overlays de GUI.
 * @details
 *  - Limpia la ventana.
 *  - Renderiza los componentes de forma (CShape) de cada actor principal.
 *  - Dibuja contorno de selección.
 *  - (Opcional, comentado) Dibuja depuración de waypoints/target.
 *  - Renderiza GUI, hace display del frame.
 */
void BaseApp::render() {
  if (m_windowPtr.isNull()) return;
  m_windowPtr->clear();
  if (m_ATrack)    m_ATrack->getComponent<CShape>()->render(m_windowPtr);
  if (m_player)    m_player->getComponent<CShape>()->render(m_windowPtr);
  if (m_racerNPC)  m_racerNPC->getComponent<CShape>()->render(m_windowPtr);
  m_engineGUI.drawSelectedOutline(m_windowPtr->m_windowPtr.get(), actorsVector);

  // ===== DEBUG DESACTIVADO: dibujar waypoints y target =====
  /*
  // Puntos verdes
  {
      sf::CircleShape dot(4.f);
      dot.setOrigin({4.f, 4.f});
      dot.setFillColor(sf::Color(0, 255, 0, 180));
      for (size_t i = 0; i < m_waypoints.size(); ++i) {
          dot.setPosition(m_waypoints[i]);
          m_windowPtr->draw(dot); // usa el wrapper (Drawable)
      }

      // Cierre del circuito (rectángulo fino del último al primero)
      if (m_waypoints.size() >= 2) {
          const sf::Vector2f a = m_waypoints.back();
          const sf::Vector2f b = m_waypoints.front();
          const sf::Vector2f d = { b.x - a.x, b.y - a.y };
          const float len = std::sqrt(d.x * d.x + d.y * d.y);
          const float angleDeg = std::atan2(d.y, d.x) * 180.f / 3.14159265f;

          sf::RectangleShape seg;
          seg.setSize({ len, 2.f });
          seg.setOrigin({ 0.f, 1.f });
          seg.setFillColor(sf::Color(0, 255, 0, 160));
          seg.setPosition(a);
          seg.setRotation(sf::degrees(angleDeg)); // SFML3
          m_windowPtr->draw(seg);
      }
  }

  // Target rojo del NPC
  if (!m_racerNPC.isNull()) {
      sf::CircleShape tgt(5.f);
      tgt.setOrigin({5.f, 5.f});
      tgt.setFillColor(sf::Color(255, 60, 60, 220));
      tgt.setPosition(m_racerNPC->getTarget());
      m_windowPtr->draw(tgt);
  }
  */

  m_windowPtr->render();
  m_engineGUI.render(m_windowPtr);
  m_windowPtr->display();
}

/**
 * @brief Libera recursos de la GUI.
 * @details Se delega a @ref EngineGUI::destroy. La ventana y otros recursos
 *  se gestionan por sus punteros inteligentes correspondientes.
 */
void BaseApp::destroy() {
  m_engineGUI.destroy();
}

/**
 * @brief Reinicia la carrera con estado inicial.
 * @details
 *  - Reposiciona jugador y NPC en la zona de salida.
 *  - Rearma WaypointFollowSystem y RaceSystem con configuración por defecto.
 *  - Reinicia la cuenta atrás y banderas de estado.
 *  - Mantiene el timing desactivado hasta que termine la nueva cuenta atrás
 *    (se activa en @ref BaseApp::update cuando corresponde).
 */
void BaseApp::resetRace() {
  if (!m_player.isNull()) {
    m_player->setPosition(sf::Vector2f(1339.926f, 300.544f));
  }
  if (!m_racerNPC.isNull()) {
    m_racerNPC->setPosition(sf::Vector2f(1320.f, 300.544f));
    m_racerNPC->enableSteering(false); // se habilita al arrancar
  }
  {
    WaypointFollowConfig wfc;
    wfc.racers = m_npcs;
    wfc.waypoints = &m_waypoints;
    wfc.arriveRadiusForAdvance = 8.f;
    wfc.corridorWidth = 24.f;
    wfc.lookAhead = 0;
    wfc.waypointNoiseRadius = 0.f;
    wfc.mistakeProb = 0.f;
    wfc.reactionDelay = 0.14f;
    wfc.cornerSlowdownEnabled = true;
    wfc.cornerMinAngleDeg = 25.f;
    wfc.cornerMaxAngleDeg = 95.f;
    wfc.cornerMinFactor = 0.78f;
    m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
    m_waypointFollowSystem->primeFromPositions();
    m_waypointFollowSystem->update(0.016f);
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
    // Nota: owner + armado + timing se activan al terminar la cuenta atrás (update()).
  }
}
