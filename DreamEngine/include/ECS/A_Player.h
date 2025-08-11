/**
 * @file
 * @brief Clase que representa al jugador controlable en el juego.
 * @details Define modos de control, parámetros de movimiento y utilidades
 *  para acceder o modificar la posición y el objetivo del jugador. Utiliza
 *  `Transform` como fuente de verdad para la posición.
 */

#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "./Actor.h"
#include "./Transform.h"

 /**
  * @enum PlayerControlMode
  * @brief Modo de control del jugador.
  */
enum class PlayerControlMode {
  Direct,     ///< Control directo por teclas (WASD u otro esquema).
  TargetSeek  ///< Movimiento automático hacia un objetivo.
};

/**
 * @class A_Player
 * @brief Actor que representa al jugador.
 * @details Permite definir el modo de control, parámetros de movimiento y
 *  manejar la posición mediante `Transform`. Hereda de `Actor`.
 */
class A_Player : public Actor {
public:
  /**
   * @brief Constructor del jugador.
   * @param name Nombre identificador del jugador.
   */
  A_Player(const std::string& name) : Actor(name) {}

  /**
   * @brief Establece el modo de control del jugador.
   * @param m Modo de control a asignar.
   */
  void setControlMode(PlayerControlMode m) { controlMode = m; }

  /**
   * @brief Obtiene el modo de control actual del jugador.
   * @return Modo de control.
   */
  PlayerControlMode getControlMode() const { return controlMode; }

  /**
   * @brief Configura la aceleración (control directo).
   * @param a Valor de aceleración en px/s².
   */
  void setAcceleration(float a) { acceleration = a; }

  /**
   * @brief Obtiene la aceleración actual.
   * @return Aceleración en px/s².
   */
  float getAcceleration() const { return acceleration; }

  /**
   * @brief Configura la fricción del jugador.
   * @param f Valor de fricción (factor de desaceleración).
   */
  void setFriction(float f) { friction = f; }

  /**
   * @brief Obtiene la fricción actual.
   * @return Fricción como factor de desaceleración.
   */
  float getFriction() const { return friction; }

  /**
   * @brief Configura la velocidad máxima permitida.
   * @param s Velocidad máxima en px/s.
   */
  void setMaxSpeed(float s) { maxSpeed = s; }

  /**
   * @brief Obtiene la velocidad máxima actual.
   * @return Velocidad máxima en px/s.
   */
  float getMaxSpeed() const { return maxSpeed; }

  /**
   * @brief Asigna la velocidad actual del jugador.
   * @param v Vector de velocidad en px/s.
   */
  void setVelocity(const sf::Vector2f& v) { velocity = v; }

  /**
   * @brief Obtiene la velocidad actual del jugador.
   * @return Vector de velocidad en px/s.
   */
  sf::Vector2f getVelocity() const { return velocity; }

  /**
   * @brief Obtiene la posición actual del jugador.
   * @details Usa el componente `Transform` como fuente de verdad.
   * @return Posición en coordenadas del mundo.
   */
  sf::Vector2f getPosition() {
    auto tr = getComponent<Transform>();
    if (tr) return tr->getPosition();
    return { 0.f, 0.f };
  }

  /**
   * @brief Establece la posición del jugador.
   * @details Aplica la posición en el componente `Transform`.
   * @param p Nueva posición en coordenadas del mundo.
   */
  void setPosition(const sf::Vector2f& p) {
    if (auto tr = getComponent<Transform>()) tr->setPosition(p);
  }

  /**
   * @brief Define un objetivo para el modo `TargetSeek`.
   * @param t Posición objetivo en coordenadas del mundo.
   */
  void setTarget(const sf::Vector2f& t) { target = t; }

  /**
   * @brief Obtiene el objetivo actual del jugador.
   * @return Posición objetivo en coordenadas del mundo.
   */
  sf::Vector2f getTarget() const { return target; }

private:
  PlayerControlMode controlMode = PlayerControlMode::Direct; ///< Modo de control.
  sf::Vector2f      velocity{ 0.f, 0.f };                    ///< Velocidad actual (px/s).
  float             acceleration = 600.f; ///< Aceleración (px/s²).
  float             friction = 6.f;       ///< Factor de fricción.
  float             maxSpeed = 250.f;     ///< Velocidad máxima (px/s).
  sf::Vector2f      target{ 0.f, 0.f };   ///< Objetivo para el modo TargetSeek.
};
