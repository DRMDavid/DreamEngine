/**
 * @file
 * @brief Sistema de seguimiento de waypoints para corredores.
 * @details Controla el comportamiento de IA para seguir un conjunto de
 *  waypoints con realismo, incluyendo retrasos de reacción, errores
 *  intencionales, variación de trayectoria y reducción de velocidad en curvas.
 */

#pragma once
#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/A_Racer.h"

 /**
  * @struct WaypointFollowConfig
  * @brief Configuración del sistema de seguimiento de waypoints.
  * @details Define la lista de corredores, el conjunto de waypoints y
  *  parámetros para el seguimiento, errores y control de velocidad en curvas.
  */
struct WaypointFollowConfig {
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers; ///< Corredores a controlar.
  const std::vector<sf::Vector2f>* waypoints = nullptr; ///< Lista de waypoints.
  float arriveRadiusForAdvance = 12.f; ///< Distancia para avanzar al siguiente waypoint.
  float reactionDelay = 0.18f; ///< Tiempo de reacción tras alcanzar un waypoint.
  float waypointNoiseRadius = 8.f; ///< Radio de ruido para aleatorizar el target.
  int   lookAhead = 1; ///< Número de waypoints por delante a considerar como target.
  float mistakeProb = 0.05f; ///< Probabilidad de error de elección de waypoint.
  float mistakeCooldown = 2.0f; ///< Tiempo mínimo entre errores.
  float corridorWidth = 80.f; ///< Ancho del corredor para detección de paso.
  bool  cornerSlowdownEnabled = true; ///< Habilita reducción de velocidad en curvas.
  float cornerMinAngleDeg = 25.f; ///< Ángulo mínimo para aplicar factor de curva.
  float cornerMaxAngleDeg = 95.f; ///< Ángulo máximo para aplicar factor de curva.
  float cornerMinFactor = 0.82f; ///< Factor mínimo de velocidad en curva pronunciada.
};

/**
 * @class WaypointFollowSystem
 * @brief Sistema para que corredores sigan una ruta de waypoints.
 * @details Actualiza el objetivo de cada corredor en base a la posición,
 *  aplica variaciones, simula errores y reduce velocidad en curvas según
 *  la configuración.
 */
class WaypointFollowSystem {
public:
  /**
   * @brief Constructor que inicializa el sistema con la configuración dada.
   * @param cfg Configuración del sistema.
   */
  WaypointFollowSystem(const WaypointFollowConfig& cfg)
    : cfg(cfg),
    indices(cfg.racers.size(), 0),
    reactTimers(cfg.racers.size(), 0.f),
    mistakeTimers(cfg.racers.size(), 0.f),
    rng(std::random_device{}()),
    uni(-1.f, 1.f),
    uni01(0.f, 1.f) {
  }

  /** @brief Destructor por defecto. */
  ~WaypointFollowSystem() = default;

  /**
   * @brief Inicializa el índice de waypoint más cercano a cada corredor.
   * @details Asigna el waypoint inicial y resetea temporizadores de reacción
   *  y errores.
   */
  void primeFromPositions() {
    if (!cfg.waypoints || cfg.waypoints->empty()) return;
    const auto& W = *cfg.waypoints;
    for (size_t i = 0; i < cfg.racers.size(); ++i) {
      auto& r = cfg.racers[i];
      if (!r) continue;
      sf::Vector2f p = r->getPosition();
      float best = std::numeric_limits<float>::max();
      size_t bestIdx = 0;
      for (size_t k = 0; k < W.size(); ++k) {
        float d = length(W[k] - p);
        if (d < best) { best = d; bestIdx = k; }
      }
      indices[i] = bestIdx;
      reactTimers[i] = 0.f;
      mistakeTimers[i] = 0.f;
      r->setTarget(W[bestIdx]);
    }
  }

  /**
   * @brief Actualiza el seguimiento de waypoints para todos los corredores.
   * @param dt Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float dt) {
    if (!cfg.waypoints || cfg.waypoints->empty()) return;
    const auto& W = *cfg.waypoints;
    if (W.empty()) return;

    for (size_t i = 0; i < cfg.racers.size(); ++i) {
      auto& r = cfg.racers[i];
      if (!r) continue;

      reactTimers[i] -= dt;
      if (reactTimers[i] > 0.f) continue;

      size_t idx = indices[i];
      size_t nextIdx = (idx + 1) % W.size();
      size_t nextNextIdx = (nextIdx + 1) % W.size();

      sf::Vector2f pos = r->getPosition();
      sf::Vector2f wp = W[idx];
      sf::Vector2f wpN = W[nextIdx];
      sf::Vector2f wpNN = W[nextNextIdx];

      sf::Vector2f seg = wpN - wp;
      float segLen2 = seg.x * seg.x + seg.y * seg.y;
      if (segLen2 <= 1e-4f) segLen2 = 1.f;

      int la = std::max(0, std::min(cfg.lookAhead, 2));
      size_t tgtIdx = (idx + static_cast<size_t>(la)) % W.size();
      sf::Vector2f target = W[tgtIdx];

      // Simulación de errores
      mistakeTimers[i] -= dt;
      if (mistakeTimers[i] <= 0.f && uni01(rng) < cfg.mistakeProb) {
        int sign = (uni01(rng) < 0.5f) ? -1 : 1;
        size_t wrong = (tgtIdx + W.size() + static_cast<size_t>(sign)) % W.size();
        target = W[wrong];
        mistakeTimers[i] = cfg.mistakeCooldown;
      }

      // Aleatorización del target
      if (cfg.waypointNoiseRadius > 0.f) {
        target.x += uni(rng) * cfg.waypointNoiseRadius;
        target.y += uni(rng) * cfg.waypointNoiseRadius;
      }

      float d_to_wp = length(wp - pos);
      float d_to_target = length(target - pos);

      sf::Vector2f rel = pos - wp;
      float t = dot(rel, seg) / segLen2;
      float tClamped = std::max(0.f, std::min(1.f, t));

      sf::Vector2f proj = { wp.x + seg.x * tClamped, wp.y + seg.y * tClamped };
      float distToLine = length(proj - pos);

      bool crossedForward = (t > 0.6f) && (distToLine <= cfg.corridorWidth);
      bool shouldAdvance =
        (d_to_wp < cfg.arriveRadiusForAdvance) ||
        (d_to_target < (cfg.arriveRadiusForAdvance * 0.7f)) ||
        crossedForward;

      if (shouldAdvance) {
        indices[i] = (idx + 1) % W.size();
        reactTimers[i] = cfg.reactionDelay;
        size_t nidx = indices[i];
        size_t ntgt = (nidx + static_cast<size_t>(la)) % W.size();
        sf::Vector2f ntarget = W[ntgt];
        if (cfg.waypointNoiseRadius > 0.f) {
          ntarget.x += uni(rng) * cfg.waypointNoiseRadius;
          ntarget.y += uni(rng) * cfg.waypointNoiseRadius;
        }
        r->setTarget(ntarget);
      }
      else {
        r->setTarget(target);
      }

      // Reducción de velocidad en curvas
      if (cfg.cornerSlowdownEnabled) {
        sf::Vector2f v1 = normalize(wpN - wp);
        sf::Vector2f v2 = normalize(wpNN - wpN);
        float dotp = dot(v1, v2);
        dotp = std::max(-1.f, std::min(1.f, dotp));
        float angleRad = std::acos(dotp);
        float angleDeg = angleRad * 57.2957795f;

        float f = 1.f;
        if (angleDeg >= cfg.cornerMaxAngleDeg) {
          f = cfg.cornerMinFactor;
        }
        else if (angleDeg > cfg.cornerMinAngleDeg) {
          float tspan = (angleDeg - cfg.cornerMinAngleDeg) /
            std::max(1.f, (cfg.cornerMaxAngleDeg - cfg.cornerMinAngleDeg));
          f = 1.f + (cfg.cornerMinFactor - 1.f) * tspan;
        }

        float baseSpd = r->getSpeed();
        float limited = baseSpd * std::min(1.f, std::max(cfg.cornerMinFactor, f));
        if (limited < baseSpd) {
          r->setSpeed(limited);
        }
      }
    }
  }

private:
  /** @brief Calcula la longitud de un vector. */
  static float length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
  }

  /** @brief Normaliza un vector. */
  static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = length(v);
    if (len <= 1e-5f) return { 0.f, 0.f };
    return { v.x / len, v.y / len };
  }

  /** @brief Producto punto entre dos vectores. */
  static float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
  }

  WaypointFollowConfig cfg; ///< Configuración del sistema.
  std::vector<size_t> indices; ///< Índice actual de waypoint por corredor.
  std::vector<float>  reactTimers; ///< Temporizadores de reacción por corredor.
  std::vector<float>  mistakeTimers; ///< Temporizadores de errores por corredor.
  std::mt19937 rng; ///< Generador de números aleatorios.
  std::uniform_real_distribution<float> uni; ///< Distribución para ruido bidimensional.
  std::uniform_real_distribution<float> uni01; ///< Distribución uniforme [0,1].
};
