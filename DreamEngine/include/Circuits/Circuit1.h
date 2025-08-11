/**
 * @file
 * @brief Define y proporciona acceso a los waypoints del circuito.
 * @details Contiene las coordenadas de los puntos que conforman el trazado de
 *  la pista, donde el primer punto corresponde a la línea de meta/salida.
 *  El vector se devuelve como referencia constante para evitar copias y
 *  mantenerlo como un recurso global de solo lectura.
 */

#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

 /**
  * @brief Obtiene la lista de waypoints del circuito.
  * @details El primer punto del vector corresponde a la meta/salida.
  *  La lista incluye los puntos originales (13) y deja comentados puntos
  *  adicionales opcionales para ampliaciones futuras del trazado.
  * @return Referencia constante al vector de `sf::Vector2f` que define el
  *  recorrido del circuito.
  */
inline const std::vector<sf::Vector2f>& getCircuitWaypoints() {
  static const std::vector<sf::Vector2f> kCircuit = {
    // --- Lista original (13) ---
    {1329.f, 249.f}, // Meta/Salida
    {1351.f, 37.f},
    {616.f, 37.f},
    {616.f, 355.f},
    {580.f, 355.f},
    {580.f, 874.f},
    {869.f, 874.f},
    {869.f, 210.f},
    {1137.f, 210.f},
    {1137.f, 835.f},
    {1303.f, 835.f},
    {1299.f, 627.f},
    {1354.f, 627.f},

   
  };
  return kCircuit;
}
