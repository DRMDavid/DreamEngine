#pragma once

#include "Prerequisites.h"
#include "Window.h"
#include "CShape.h"
#include <vector>
#include "ECS/Actor.h"
#include "EngineGUI.h"
#include "ECS/A_Player.h"
#include "ECS/A_Racer.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/SteeringSystem.h"
#include "Systems/WaypointFollowSystem.h"
#include "Systems/RaceSystem.h"
#include "Components/Countdown.h"

class BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	int run();
	bool init();
	void update();
	void render();
	void destroy();

private:
	EngineUtilities::TSharedPointer<Window>   m_windowPtr;
	EngineUtilities::TSharedPointer<Actor>    m_ACircle;
	EngineUtilities::TSharedPointer<Actor>    m_ATrack;
	EngineUtilities::TSharedPointer<A_Racer>  m_racerNPC;
	EngineUtilities::TSharedPointer<A_Player> m_player;
	std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_npcs;
	std::vector<EngineUtilities::TSharedPointer<Actor>> actorsVector;
	EngineGUI m_engineGUI;
	std::vector<sf::Vector2f> m_waypoints;
	EngineUtilities::TUniquePtr<PlayerInputSystem>    m_playerInputSystem;
	EngineUtilities::TUniquePtr<SteeringSystem>       m_steeringSystem;
	EngineUtilities::TUniquePtr<WaypointFollowSystem> m_waypointFollowSystem;
	EngineUtilities::TUniquePtr<RaceSystem>           m_raceSystem;
	Countdown m_countdown{ 3.f };
	bool m_raceArmed = false;
	bool m_raceLive = false;
	bool m_raceFinished = false;
	int  m_finalPlace = -1;
	int  m_lapsToWin = 3;
	bool m_npcFinished = false;
	float m_sharedMaxSpeed = 260.f;
	float m_npcSpeedFactor = 0.95f;

	void resetRace();
};
