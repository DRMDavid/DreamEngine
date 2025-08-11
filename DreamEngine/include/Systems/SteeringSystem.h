/**
 * @file
 * @brief Sistema de steering para corredores.
 * @details Implementa comportamientos de movimiento como "arrive" y "pursuit"
 *  para actores del tipo `A_Racer`, permitiendo un control más realista de IA.
 */

#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/A_Racer.h"
#include "ECS/Transform.h"

 /**
  * @struct SteeringConfig
  * @brief Configuración para el sistema de steering.
  * @details Contiene la lista de corredores y parámetros como rango de seek.
  */
struct SteeringConfig {
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers; ///< Lista de corredores a controlar.
  float seekRange = 10.f; ///< Distancia mínima para considerar llegada.
};

/**
 * @class SteeringSystem
 * @brief Sistema que gestiona el steering de corredores.
 * @details Permite actualizar el comportamiento de movimiento de corredores
 *  según el modo seleccionado (Seek, Arrive, Pursuit).
 */
class SteeringSystem {
public:
  /**
   * @brief Constructor que inicializa el sistema con la configuración dada.
   * @param cfg Configuración con lista de corredores y parámetros.
   */
  SteeringSystem(const SteeringConfig& cfg) : cfg(cfg) {}

  /** @brief Destructor por defecto. */
  ~SteeringSystem() = default;

  /**
   * @brief Actualiza el steering de todos los corredores.
   * @param dt Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float dt);

private:
  SteeringConfig cfg; ///< Configuración interna del sistema.

  /**
   * @brief Comportamiento "Arrive".
   * @details Calcula la dirección y velocidad ajustada para llegar suavemente
   *  a un objetivo.
   * @param pos Posición actual.
   * @param target Posición objetivo.
   * @param speed Velocidad máxima.
   * @param arriveRadius Radio dentro del cual comienza la desaceleración.
   * @return Vector de dirección ajustado para la llegada.
   */
  sf::Vector2f arrive_L(const sf::Vector2f& pos,
    const sf::Vector2f& target,
    float speed, float arriveRadius);

  /**
   * @brief Comportamiento "Pursuit".
   * @details Calcula la dirección hacia la posición futura de un objetivo
   *  en movimiento, anticipando su desplazamiento.
   * @param self Corredor que persigue.
   * @param target Corredor objetivo.
   * @param speed Velocidad máxima del perseguidor.
   * @param maxPrediction Tiempo máximo de predicción (segundos).
   * @return Vector de dirección hacia la posición predicha del objetivo.
   */
  sf::Vector2f pursuit_L(const A_Racer& self,
    const A_Racer& target,
    float speed, float maxPrediction);
};
