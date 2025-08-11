/**
 * @file
 * @brief Implementación del sistema de gestión de carreras (RaceSystem).
 * @details Controla el conteo de vueltas, tiempos por vuelta, posiciones de carrera
 * y lógica de checkpoints para los actores participantes.
 */

#include "Systems/RaceSystem.h"
#include <algorithm>
#include <cmath>
#include <limits>

 // ============================================================================
 // Constructor
 // ============================================================================
 /**
  * @brief Constructor de RaceSystem.
  * @param inCfg Configuración inicial de la carrera (actores, waypoints, etc.).
  * @details Inicializa métricas del circuito, estructuras de datos para progreso,
  * tiempos y vueltas, además de determinar el checkpoint inicial más cercano
  * para cada actor.
  */
RaceSystem::RaceSystem(const RaceConfig& inCfg)
  : cfg(inCfg) {

  buildCircuitMeter();

  const size_t N = cfg.actors.size();
  laps.clear();        laps.resize(N);
  progress.clear();    progress.resize(N, 0.f);
  elapsed.clear();     elapsed.resize(N, 0.f);
  lastS.clear();       lastS.resize(N, 0.f);

  lapCooldown = lapDebounceSec;
  lapArmed = false;

  timingActive = false;
  playerLapTime = 0.f;
  playerBestLap = 0.f;
  bestLapValid = false;

  if (cfg.waypoints && !cfg.waypoints->empty()) {
    const auto& W = *cfg.waypoints;
    for (size_t i = 0; i < N; ++i) {
      auto& a = cfg.actors[i];

      Lap lc{};
      lc.lap = 0;
      lc.checkpoint = 0;

      if (!a.isNull()) {
        sf::Vector2f p = getActorPos(a);
        float best = std::numeric_limits<float>::max();
        size_t bestIdx = 0;
        for (size_t k = 0; k < W.size(); ++k) {
          float d = dist(p, W[k]);
          if (d < best) { best = d; bestIdx = k; }
        }
        lc.checkpoint = static_cast<int>(bestIdx);
        float s0 = sAlongPath(p);
        lastS[i] = s0;
        progress[i] = s0;
      }
      else {
        progress[i] = 0.f;
        lastS[i] = 0.f;
      }

      elapsed[i] = 0.f;
      laps[i] = lc;
    }
  }
}

// ============================================================================
// Actualización por frame
// ============================================================================
/**
 * @brief Actualiza el estado de la carrera.
 * @param dt Tiempo transcurrido desde el último frame (segundos).
 * @details Calcula el avance de cada actor en el circuito, detecta paso por
 * checkpoints, actualiza progreso y cronometra las vueltas del jugador.
 */
void RaceSystem::update(float dt) {
  if (!cfg.waypoints || cfg.waypoints->size() < 2) return;
  const auto& W = *cfg.waypoints;
  const size_t Nw = W.size();
  const size_t Na = cfg.actors.size();
  if (Nw < 2 || Na == 0) return;

  if (lapCooldown > 0.f) {
    lapCooldown -= dt;
    if (lapCooldown < 0.f) lapCooldown = 0.f;
  }

  if (timingActive) {
    playerLapTime += dt;
  }

  for (size_t i = 0; i < Na; ++i) {
    auto& a = cfg.actors[i];
    if (a.isNull()) continue;

    elapsed[i] += dt;
    sf::Vector2f p = getActorPos(a);

    int idx = (laps[i].checkpoint < 0) ? 0 : laps[i].checkpoint;
    int next = (idx + 1) % static_cast<int>(Nw);

    sf::Vector2f wp = W[static_cast<size_t>(idx)];
    sf::Vector2f wpN = W[static_cast<size_t>(next)];

    float dToCurr = dist(p, wp);
    bool near = (dToCurr <= cfg.checkpointRadius);

    sf::Vector2f seg = { wpN.x - wp.x, wpN.y - wp.y };
    sf::Vector2f rel = { p.x - wp.x,  p.y - wp.y };
    bool forward = (seg.x * rel.x + seg.y * rel.y) > 0.f;

    if (near && forward) {
      laps[i].checkpoint = next;
    }

    float sNow = sAlongPath(p);
    float sPrev = lastS[i];

    if (totalLen > 0.f) {
      float ds = sNow - sPrev;

      if (i == static_cast<size_t>(lapOwnerIndex)) {
        if (ds < -0.5f * totalLen && lapCooldown <= 0.f) {
          if (!lapArmed) {
            lapArmed = true;
            lapCooldown = lapDebounceSec;
            playerLapTime = 0.f;
          }
          else {
            laps[i].lap += 1;

            if (timingActive) {
              float thisLap = playerLapTime;
              if (thisLap > 0.f) {
                if (!bestLapValid || thisLap < playerBestLap) {
                  playerBestLap = thisLap;
                  bestLapValid = true;
                }
              }
              playerLapTime = 0.f;
            }
            lapCooldown = lapDebounceSec;
          }
        }
      }
    }

    lastS[i] = sNow;
    progress[i] = static_cast<float>(laps[i].lap) * totalLen + sNow;
  }
}

// ============================================================================
// Orden de posiciones
// ============================================================================
/**
 * @brief Devuelve el orden de los competidores.
 * @return Vector con los índices de actores ordenados por progreso descendente.
 */
std::vector<int> RaceSystem::getStandings() const {
  std::vector<int> order;
  order.reserve(progress.size());
  for (size_t i = 0; i < progress.size(); ++i)
    order.push_back(static_cast<int>(i));

  std::sort(order.begin(), order.end(), [&](int L, int R) {
    return progress[static_cast<size_t>(L)] > progress[static_cast<size_t>(R)];
    });
  return order;
}

// ============================================================================
// Funciones utilitarias privadas
// ============================================================================
/**
 * @brief Obtiene la posición actual de un actor.
 * @param a Puntero compartido al actor.
 * @return Posición en coordenadas 2D.
 */
sf::Vector2f RaceSystem::getActorPos(const EngineUtilities::TSharedPointer<Actor>& a) {
  auto tr = a->getComponent<Transform>();
  if (tr) {
    return tr->getPosition();
  }
  return sf::Vector2f{ 0.f, 0.f };
}

/**
 * @brief Calcula el progreso relativo de un punto sobre un segmento.
 * @param p Punto a evaluar.
 * @param a Punto inicial del segmento.
 * @param b Punto final del segmento.
 * @return Valor entre 0 y 1 indicando la posición relativa sobre el segmento.
 */
float RaceSystem::segProgress(const sf::Vector2f& p,
  const sf::Vector2f& a,
  const sf::Vector2f& b) const {
  sf::Vector2f ab{ b.x - a.x, b.y - a.y };
  sf::Vector2f ap{ p.x - a.x, p.y - a.y };
  float ab2 = ab.x * ab.x + ab.y * ab.y;
  if (ab2 <= 1e-6f) return 0.f;
  float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
  if (t < 0.f) t = 0.f;
  if (t > 1.f) t = 1.f;
  return t;
}

/**
 * @brief Calcula la distancia recorrida a lo largo del circuito hasta el punto dado.
 * @param p Punto en el circuito.
 * @return Distancia acumulada desde la línea de meta hasta la proyección más cercana.
 */
float RaceSystem::sAlongPath(const sf::Vector2f& p) const {
  if (!cfg.waypoints || cfg.waypoints->size() < 2) return 0.f;
  const auto& W = *cfg.waypoints;
  const size_t N = W.size();

  float bestD2 = std::numeric_limits<float>::max();
  float bestS = 0.f;

  for (size_t i = 0; i < N; ++i) {
    size_t j = (i + 1) % N;
    if (!closedLoop && j == 0) break;

    const sf::Vector2f& A = W[i];
    const sf::Vector2f& B = W[j];
    float t = segProgress(p, A, B);

    sf::Vector2f proj{
      A.x + (B.x - A.x) * t,
      A.y + (B.y - A.y) * t
    };
    float dx = proj.x - p.x, dy = proj.y - p.y;
    float d2 = dx * dx + dy * dy;
    if (d2 < bestD2) {
      bestD2 = d2;
      float base = (i < prefix.size() ? prefix[i] : 0.f);
      float segLen = dist(A, B);
      bestS = base + t * segLen;
    }
  }

  if (totalLen > 0.f) {
    if (bestS >= totalLen) bestS = std::fmod(bestS, totalLen);
    if (bestS < 0.f)       bestS += totalLen;
  }
  return bestS;
}

/**
 * @brief Construye la métrica acumulada de distancias entre waypoints.
 */
void RaceSystem::buildCircuitMeter() {
  prefix.clear();
  totalLen = 0.f;
  closedLoop = true;

  if (!cfg.waypoints || cfg.waypoints->size() < 2) return;
  const auto& W = *cfg.waypoints;
  const size_t N = W.size();

  prefix.resize(N, 0.f);
  for (size_t i = 1; i < N; ++i) {
    float seg = dist(W[i - 1], W[i]);
    totalLen += seg;
    prefix[i] = totalLen;
  }

  if (closedLoop) {
    totalLen += dist(W[N - 1], W[0]);
  }
}

// ============================================================================
// Control explícito del conteo de vueltas
// ============================================================================
/**
 * @brief Activa o desactiva el contador de vueltas inicial.
 * @param armed Estado del armado del contador.
 */
void RaceSystem::armLapCounter(bool armed) {
  lapArmed = armed;
  lapCooldown = lapDebounceSec;
  if (armed) playerLapTime = 0.f;
}

/**
 * @brief Define qué actor es el propietario del conteo de vueltas.
 * @param idx Índice del actor (0 = jugador).
 */
void RaceSystem::setLapOwnerIndex(int idx) {
  lapOwnerIndex = idx;
}
