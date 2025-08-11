/**
 * @file
 * @brief Sistema de entrada para el jugador.
 * @details Gestiona y procesa la entrada de usuario para un objeto `A_Player`,
 *  permitiendo modos de control directo o de búsqueda de objetivo (TargetSeek).
 */

#pragma once
#include "Prerequisites.h"
#include "ECS/A_Player.h"

 /**
  * @struct PlayerInputConfig
  * @brief Configuración para el sistema de entrada del jugador.
  * @details Contiene las referencias necesarias para manejar la entrada
  *  del jugador, incluyendo el jugador objetivo y la ventana de render.
  */
struct PlayerInputConfig {
  EngineUtilities::TSharedPointer<A_Player> player; ///< Puntero al jugador controlado.
  sf::RenderWindow* renderWindow = nullptr; ///< Puntero a la ventana de renderizado.
};

/**
 * @class PlayerInputSystem
 * @brief Sistema que procesa la entrada del jugador.
 * @details Se encarga de interpretar las acciones del usuario y traducirlas
 *  en movimientos y acciones sobre un `A_Player`, según el modo de control.
 */
class PlayerInputSystem {
public:
  /**
   * @brief Constructor que recibe la configuración.
   * @param cfg Estructura de configuración con jugador y ventana.
   */
  PlayerInputSystem(const PlayerInputConfig& cfg) : cfg(cfg) {}

  /** @brief Destructor por defecto. */
  ~PlayerInputSystem() = default;

  /**
   * @brief Actualiza el estado del sistema de entrada.
   * @param dt Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float dt);

private:
  PlayerInputConfig cfg; ///< Configuración del sistema.

  /**
   * @brief Procesa la entrada en modo de control directo.
   * @param dt Tiempo transcurrido desde el último frame (segundos).
   */
  void updateDirect(float dt);

  /**
   * @brief Procesa la entrada en modo TargetSeek.
   */
  void updateTargetSeek();
};
