/**
 * @file
 * @brief Implementación del sistema de entrada para el jugador.
 * @details Gestiona la entrada desde teclado y ratón para controlar un
 *  `A_Player` en modo directo o en modo de seguimiento de objetivo.
 */

#include "Systems/PlayerInputSystem.h"
#include "ECS/Transform.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

 /**
  * @brief Calcula la longitud de un vector 2D.
  * @param v Vector a medir.
  * @return Magnitud del vector.
  */
static inline float vecLength(const sf::Vector2f& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}

/**
 * @brief Normaliza un vector 2D.
 * @param v Vector a normalizar.
 * @return Vector normalizado (o {0,0} si la longitud es muy pequeña).
 */
static inline sf::Vector2f vecNormalize(const sf::Vector2f& v) {
  float L = vecLength(v);
  if (L <= 1e-6f) return { 0.f, 0.f };
  return { v.x / L, v.y / L };
}

/**
 * @brief Actualiza la entrada del jugador según el modo de control configurado.
 * @param dt Tiempo transcurrido desde el último frame (segundos).
 */
void PlayerInputSystem::update(float dt) {
  if (!cfg.player) return;
  if (cfg.player->getControlMode() == PlayerControlMode::Direct)
    updateDirect(dt);
  else
    updateTargetSeek();
}

/**
 * @brief Procesa la entrada en modo de control directo (teclas WASD / flechas).
 * @param dt Tiempo transcurrido desde el último frame (segundos).
 */
void PlayerInputSystem::updateDirect(float dt) {
  sf::Vector2f a(0.f, 0.f);

  // Movimiento vertical
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    a.y -= cfg.player->getAcceleration();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    a.y += cfg.player->getAcceleration();

  // Movimiento horizontal
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    a.x -= cfg.player->getAcceleration();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    a.x += cfg.player->getAcceleration();

  // Actualizar velocidad
  auto v = cfg.player->getVelocity();
  v += a * dt;

  // Aplicar fricción si no hay aceleración
  if (a.x == 0.f && a.y == 0.f) {
    v *= std::max(0.f, 1.f - cfg.player->getFriction() * dt);
  }

  // Limitar a la velocidad máxima
  float sp = vecLength(v);
  if (sp > cfg.player->getMaxSpeed())
    v = vecNormalize(v) * cfg.player->getMaxSpeed();

  // Actualizar posición
  auto p = cfg.player->getPosition();
  p += v * dt;

  // Guardar nueva velocidad y posición
  cfg.player->setVelocity(v);
  cfg.player->setPosition(p);
}

/**
 * @brief Procesa la entrada en modo seguimiento de objetivo (TargetSeek).
 * @details Establece el objetivo del jugador en la posición del ratón al hacer clic.
 */
void PlayerInputSystem::updateTargetSeek() {
  if (!cfg.renderWindow) return;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    auto mp = sf::Mouse::getPosition(*cfg.renderWindow);
    auto world = cfg.renderWindow->mapPixelToCoords(mp);
    cfg.player->setTarget({ world.x, world.y });
  }
}
