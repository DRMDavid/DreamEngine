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

    // --- NUEVOS (comentados; para futura ampliación del trazado) ---
    /** {1342.f, 213.f},{1342.f, 147.f},{1342.f, 94.f},{1342.f, 53.f},
     {1249.f, 35.f},{1161.f, 35.f},{1085.f, 35.f},{993.f, 35.f},
     {903.f, 35.f},{804.f, 35.f},{696.f, 35.f},{641.f, 35.f},
     {599.f, 106.f},{599.f, 189.f},{599.f, 243.f},
     {560.f, 400.f},{560.f, 476.f},{560.f, 533.f},
     {631.f, 533.f},{631.f, 639.f},{631.f, 701.f},{631.f, 775.f},{631.f, 849.f},
     {717.f, 849.f},{795.f, 849.f},
     {864.f, 700.f},{864.f, 691.f},{864.f, 511.f},{864.f, 405.f},{864.f, 274.f},
     {980.f, 188.f},
     {1137.f, 290.f},{1137.f, 385.f},{1137.f, 519.f},{1137.f, 659.f},{1137.f, 741.f},
     {1192.f, 812.f},
     {1290.f, 710.f},{1290.f, 664.f},
     {1329.f, 490.f},{1329.f, 421.f},{1329.f, 254.f},{1329.f, 249.f} */
  };
  return kCircuit;
}
