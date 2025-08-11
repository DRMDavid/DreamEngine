#include "Systems/PlayerInputSystem.h"
#include "ECS/Transform.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

static inline float vecLength(const sf::Vector2f& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}
static inline sf::Vector2f vecNormalize(const sf::Vector2f& v) {
  float L = vecLength(v);
  if (L <= 1e-6f) return { 0.f, 0.f };
  return { v.x / L, v.y / L };
}

void PlayerInputSystem::update(float dt) {
  if (!cfg.player) return;
  if (cfg.player->getControlMode() == PlayerControlMode::Direct) updateDirect(dt);
  else updateTargetSeek();
}

void PlayerInputSystem::updateDirect(float dt) {
  sf::Vector2f a(0.f, 0.f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    a.y -= cfg.player->getAcceleration();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    a.y += cfg.player->getAcceleration();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    a.x -= cfg.player->getAcceleration();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    a.x += cfg.player->getAcceleration();

  auto v = cfg.player->getVelocity();
  v += a * dt;

  if (a.x == 0.f && a.y == 0.f) {
    v *= std::max(0.f, 1.f - cfg.player->getFriction() * dt);
  }

  float sp = vecLength(v);
  if (sp > cfg.player->getMaxSpeed()) v = vecNormalize(v) * cfg.player->getMaxSpeed();

  auto p = cfg.player->getPosition();
  p += v * dt;

  cfg.player->setVelocity(v);
  cfg.player->setPosition(p);
}

void PlayerInputSystem::updateTargetSeek() {
  if (!cfg.renderWindow) return;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    auto mp = sf::Mouse::getPosition(*cfg.renderWindow);
    auto world = cfg.renderWindow->mapPixelToCoords(mp);
    cfg.player->setTarget({ world.x, world.y });
  }
}