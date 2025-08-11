/**
 * @file
 * @brief Clase principal de la aplicación base del motor/juego.
 * @details Gestiona la inicialización, bucle principal, sistemas y
 *  recursos para la simulación de carreras, incluyendo jugadores y NPCs.
 */

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

 /**
  * @class BaseApp
  * @brief Clase que gestiona la ejecución principal del juego.
  * @details Contiene el ciclo de vida completo de la aplicación:
  *  inicialización, actualización, renderizado y destrucción, además de
  *  gestionar todos los sistemas y actores implicados en la simulación.
  */
class BaseApp {
public:
  /** @brief Constructor por defecto. */
  BaseApp() = default;

  /** @brief Destructor que libera los recursos de la aplicación. */
  ~BaseApp();

  /**
   * @brief Ejecuta el bucle principal de la aplicación.
   * @return Código de salida.
   */
  int run();

  /**
   * @brief Inicializa los recursos, sistemas y actores.
   * @return true si la inicialización fue exitosa.
   */
  bool init();

  /** @brief Actualiza la lógica del juego y sistemas. */
  void update();

  /** @brief Renderiza la escena y elementos gráficos. */
  void render();

  /** @brief Libera recursos y destruye la aplicación. */
  void destroy();

private:
  EngineUtilities::TSharedPointer<Window>   m_windowPtr; ///< Puntero a la ventana principal.
  EngineUtilities::TSharedPointer<Actor>    m_ACircle;   ///< Actor para un círculo (ej. marcador).
  EngineUtilities::TSharedPointer<Actor>    m_ATrack;    ///< Actor para la pista.
  EngineUtilities::TSharedPointer<A_Racer>  m_racerNPC;  ///< Ejemplo de NPC corredor.
  EngineUtilities::TSharedPointer<A_Player> m_player;    ///< Jugador principal.
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_npcs; ///< Lista de NPCs corredores.
  std::vector<EngineUtilities::TSharedPointer<Actor>> actorsVector; ///< Todos los actores de la escena.
  EngineGUI m_engineGUI; ///< Interfaz gráfica del motor.
  std::vector<sf::Vector2f> m_waypoints; ///< Lista de waypoints para la carrera.

  EngineUtilities::TUniquePtr<PlayerInputSystem>    m_playerInputSystem;    ///< Sistema de entrada del jugador.
  EngineUtilities::TUniquePtr<SteeringSystem>       m_steeringSystem;       ///< Sistema de steering para NPCs.
  EngineUtilities::TUniquePtr<WaypointFollowSystem> m_waypointFollowSystem; ///< Sistema de seguimiento de waypoints.
  EngineUtilities::TUniquePtr<RaceSystem>           m_raceSystem;           ///< Sistema de control de carrera.
  Countdown m_countdown{ 3.f }; ///< Temporizador de cuenta regresiva antes de iniciar la carrera.

  bool m_raceArmed = false;    ///< Indica si la carrera está lista para comenzar.
  bool m_raceLive = false;     ///< Indica si la carrera está en progreso.
  bool m_raceFinished = false; ///< Indica si la carrera ha finalizado.
  int  m_finalPlace = -1;      ///< Posición final del jugador.
  int  m_lapsToWin = 3;        ///< Número de vueltas necesarias para ganar.
  bool m_npcFinished = false;  ///< Indica si un NPC ha terminado la carrera.
  float m_sharedMaxSpeed = 260.f; ///< Velocidad máxima compartida por jugadores y NPCs.
  float m_npcSpeedFactor = 0.95f; ///< Factor de velocidad de NPCs respecto al jugador.

  /** @brief Reinicia el estado de la carrera a valores iniciales. */
  void resetRace();
};
