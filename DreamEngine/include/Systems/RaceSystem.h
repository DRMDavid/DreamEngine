/**
 * @file
 * @brief Sistema de gestión de carreras.
 * @details Controla el progreso de los actores en un circuito, midiendo
 *  vueltas, tiempos y posiciones. Puede manejar un circuito cerrado o abierto,
 *  así como registrar y validar el mejor tiempo del jugador.
 */

#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "../Components/Lap.h"
#include "ECS/A_Racer.h"
#include <vector>

 /**
  * @struct RaceConfig
  * @brief Configuración inicial del sistema de carreras.
  * @details Define los actores participantes, los puntos de control y
  *  parámetros de detección de checkpoints.
  */
struct RaceConfig {
  std::vector<EngineUtilities::TSharedPointer<Actor>> actors; ///< Lista de actores en carrera.
  const std::vector<sf::Vector2f>* waypoints = nullptr; ///< Puntero a la lista de checkpoints.
  float checkpointRadius = 12.f; ///< Radio para validar paso por checkpoint.
};

/**
 * @class RaceSystem
 * @brief Sistema que gestiona el conteo de vueltas y tiempos.
 * @details Registra el progreso de cada competidor, calcula posiciones,
 *  controla el timing y detecta vueltas completas.
 */
class RaceSystem {
public:
  /**
   * @brief Constructor que inicializa la carrera con configuración dada.
   * @param cfg Configuración de la carrera.
   */
  RaceSystem(const RaceConfig& cfg);

  /** @brief Destructor por defecto. */
  ~RaceSystem() = default;

  /**
   * @brief Actualiza el estado de la carrera.
   * @param dt Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float dt);

  /**
   * @brief Obtiene la clasificación actual.
   * @return Vector con los índices de actores ordenados por posición.
   */
  std::vector<int> getStandings() const;

  /**
   * @brief Obtiene los datos de vuelta de un competidor.
   * @param i Índice del competidor.
   * @return Referencia constante a la estructura `Lap`.
   */
  const Lap& getLapData(size_t i) const { return laps[i]; }

  /**
   * @brief Activa o desactiva el conteo de tiempos.
   * @param b true para activar, false para desactivar.
   */
  void setTimingActive(bool b) { timingActive = b; }

  /**
   * @brief Obtiene el tiempo actual de la vuelta del jugador.
   * @return Tiempo en segundos.
   */
  float getPlayerCurrentLapTime() const { return playerLapTime; }

  /**
   * @brief Obtiene el mejor tiempo de vuelta del jugador.
   * @return Tiempo en segundos o -1 si no existe vuelta válida.
   */
  float getPlayerBestLapTime() const { return (bestLapValid ? playerBestLap : -1.f); }

  /**
   * @brief Indica si hay un mejor tiempo de vuelta válido.
   * @return true si existe mejor tiempo válido.
   */
  bool hasBestLap() const { return bestLapValid; }

  /**
   * @brief Arma o desarma el conteo de la primera vuelta.
   * @param armed true para armar, false para desarmar.
   */
  void armLapCounter(bool armed);

  /**
   * @brief Define quién posee el conteo de vueltas.
   * @param idx Índice del actor (0 = jugador).
   */
  void setLapOwnerIndex(int idx);

private:
  RaceConfig cfg; ///< Configuración de la carrera.
  std::vector<Lap> laps; ///< Datos de vueltas por competidor.
  std::vector<float> progress; ///< Progreso total de cada competidor.
  std::vector<float> elapsed; ///< Tiempo transcurrido por competidor.
  std::vector<float> lastS; ///< Última distancia registrada en el circuito.
  int lapOwnerIndex = 0; ///< Índice del actor dueño del conteo.
  float lapDebounceSec = 0.35f; ///< Tiempo mínimo entre detecciones de vuelta.
  float lapCooldown = 0.f; ///< Temporizador para evitar conteo doble.
  bool lapArmed = false; ///< Estado armado del conteo inicial.
  bool timingActive = false; ///< Indica si el cronometraje está activo.
  float playerLapTime = 0.f; ///< Tiempo actual de vuelta del jugador.
  float playerBestLap = 0.f; ///< Mejor tiempo de vuelta del jugador.
  bool bestLapValid = false; ///< Indica si el mejor tiempo es válido.
  std::vector<float> prefix; ///< Distancia acumulada previa por segmento.
  float totalLen = 0.f; ///< Longitud total del circuito.
  bool closedLoop = true; ///< Indica si el circuito es cerrado.

  /**
   * @brief Obtiene la posición de un actor.
   * @param a Puntero compartido al actor.
   * @return Vector de posición en el mundo.
   */
  static sf::Vector2f getActorPos(const EngineUtilities::TSharedPointer<Actor>& a);

  /**
   * @brief Calcula el progreso dentro de un segmento.
   * @param p Posición actual.
   * @param a Punto inicial del segmento.
   * @param b Punto final del segmento.
   * @return Progreso relativo [0.0, 1.0].
   */
  float segProgress(const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b) const;

  /**
   * @brief Calcula la distancia recorrida a lo largo del circuito.
   * @param p Posición actual.
   * @return Distancia recorrida en unidades del mundo.
   */
  float sAlongPath(const sf::Vector2f& p) const;

  /** @brief Calcula la distancia total del circuito y la estructura de segmentos. */
  void buildCircuitMeter();

  /**
   * @brief Calcula la distancia entre dos puntos.
   * @param A Punto A.
   * @param B Punto B.
   * @return Distancia en unidades del mundo.
   */
  static float dist(const sf::Vector2f& A, const sf::Vector2f& B) {
    float dx = B.x - A.x, dy = B.y - A.y;
    return std::sqrt(dx * dx + dy * dy);
  }
};
