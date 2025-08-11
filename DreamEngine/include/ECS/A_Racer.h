/**
 * @file
 * @brief Clase que representa un competidor (Racer) en el juego.
 * @details Contiene propiedades y métodos para manejar el estado lógico de
 *  movimiento, modo de steering y parámetros de persecución. Hereda de Actor.
 */

#pragma once
#include "Actor.h"
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"

 /**
  * @enum SteeringMode
  * @brief Modos de steering para movimiento de la IA.
  */
enum class SteeringMode {
  Seek,   ///< Movimiento hacia un objetivo.
  Arrive, ///< Movimiento con desaceleración al llegar.
  Pursuit ///< Persecución de un objetivo en movimiento.
};

/**
 * @class A_Racer
 * @brief Representa un corredor controlado por IA o jugador.
 * @details Permite configurar posición lógica, objetivo, velocidad y
 *  parámetros de steering, incluyendo persecución.
 */
class A_Racer : public Actor {
public:
  /**
   * @brief Constructor del corredor.
   * @param name Nombre identificador del corredor.
   */
  A_Racer(const std::string& name);

  /**
   * @brief Actualiza el estado lógico del corredor.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float deltaTime) override;

  /**
   * @brief Establece el lugar actual en la carrera.
   * @param newPlace Posición (1 = primero, etc.).
   */
  void setPlace(int newPlace) { place = newPlace; }

  /**
   * @brief Obtiene el lugar actual en la carrera.
   * @return Lugar en la carrera.
   */
  int getPlace() const { return place; }

  /**
   * @brief Define la posición lógica del corredor.
   * @param pos Posición en coordenadas del mundo.
   */
  void setPosition(const sf::Vector2f& pos) { logicPosition = pos; }

  /**
   * @brief Obtiene la posición lógica del corredor.
   * @return Posición en coordenadas del mundo.
   */
  sf::Vector2f getPosition() const { return logicPosition; }

  /**
   * @brief Define el objetivo lógico del corredor.
   * @param waypoint Posición objetivo en coordenadas del mundo.
   */
  void setTarget(const sf::Vector2f& waypoint) { logicTarget = waypoint; }

  /**
   * @brief Obtiene el objetivo lógico del corredor.
   * @return Posición objetivo en coordenadas del mundo.
   */
  sf::Vector2f getTarget() const { return logicTarget; }

  /**
   * @brief Establece la velocidad de desplazamiento.
   * @param s Velocidad en px/s.
   */
  void setSpeed(float s) { speed = s; }

  /**
   * @brief Obtiene la velocidad actual.
   * @return Velocidad en px/s.
   */
  float getSpeed() const { return speed; }

  /**
   * @brief Activa o desactiva el steering.
   * @param enabled true para activar, false para desactivar.
   */
  void enableSteering(bool enabled) { steeringEnabled = enabled; }

  /**
   * @brief Indica si el steering está habilitado.
   * @return true si está habilitado, false si no.
   */
  bool isSteeringEnabled() const { return steeringEnabled; }

  /**
   * @brief Define el radio de llegada en modo Arrive.
   * @param radius Radio en unidades del mundo.
   */
  void setArriveRadius(float radius) { arriveRadius = radius; }

  /**
   * @brief Obtiene el radio de llegada.
   * @return Radio en unidades del mundo.
   */
  float getArriveRadius() const { return arriveRadius; }

  /**
   * @brief Establece el modo de steering.
   * @param m Modo a utilizar.
   */
  void setMode(SteeringMode m) { mode = m; }

  /**
   * @brief Obtiene el modo de steering actual.
   * @return Modo de steering.
   */
  SteeringMode getMode() const { return mode; }

  /**
   * @brief Define el objetivo de persecución.
   * @param t Puntero débil al corredor objetivo.
   */
  void setPursuitTarget(const EngineUtilities::TWeakPointer<A_Racer>& t) { pursuitTarget = t; }

  /**
   * @brief Obtiene el objetivo de persecución.
   * @return Puntero débil al corredor objetivo.
   */
  EngineUtilities::TWeakPointer<A_Racer> getPursuitTarget() const { return pursuitTarget; }

  /**
   * @brief Establece el tiempo máximo de predicción para Pursuit.
   * @param p Tiempo en segundos.
   */
  void setMaxPrediction(float p) { maxPrediction = p; }

  /**
   * @brief Obtiene el tiempo máximo de predicción para Pursuit.
   * @return Tiempo en segundos.
   */
  float getMaxPrediction() const { return maxPrediction; }

private:
  int         place = 0; ///< Lugar actual en la carrera.

  sf::Vector2f logicPosition{ 0.f, 0.f }; ///< Posición lógica.
  sf::Vector2f logicTarget{ 0.f, 0.f };   ///< Objetivo lógico.
  float        speed = 100.f;             ///< Velocidad de desplazamiento (px/s).

  bool         steeringEnabled = true;    ///< Indicador de steering activo.
  float        arriveRadius = 10.f;       ///< Radio para modo Arrive.
  SteeringMode mode = SteeringMode::Seek; ///< Modo de steering actual.

  EngineUtilities::TWeakPointer<A_Racer> pursuitTarget; ///< Objetivo de persecución.
  float        maxPrediction = 0.5f; ///< Tiempo máx. de predicción en Pursuit (segundos).
};
