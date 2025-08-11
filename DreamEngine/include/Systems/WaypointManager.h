/**
 * @file
 * @brief Clase para la gestión de waypoints en un circuito o trayectoria.
 * @details Permite almacenar y acceder a una lista de puntos de control
 *  (`sf::Vector2f`) que definen una ruta en el mundo del juego.
 */

#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

 /**
  * @class WaypointManager
  * @brief Gestor de waypoints.
  * @details Facilita la configuración y acceso a una lista de puntos de
  *  control utilizados para navegación, IA o trazado de recorridos.
  */
class WaypointManager {
public:
  /**
   * @brief Establece la lista de waypoints.
   * @param pts Vector de posiciones que definen los waypoints.
   */
  void setWaypoints(const std::vector<sf::Vector2f>& pts) {
    waypoints_ = pts;
  }

  /**
   * @brief Obtiene la lista de waypoints actuales.
   * @return Referencia constante al vector de waypoints.
   */
  const std::vector<sf::Vector2f>& waypoints() const { return waypoints_; }

private:
  std::vector<sf::Vector2f> waypoints_; ///< Lista de puntos de control.
};
