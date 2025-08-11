#include "Systems/SteeringSystem.h"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>

namespace {
  inline sf::Vector2f vadd(const sf::Vector2f& a, const sf::Vector2f& b) { return { a.x + b.x, a.y + b.y }; }
  inline sf::Vector2f vsub(const sf::Vector2f& a, const sf::Vector2f& b) { return { a.x - b.x, a.y - b.y }; }
  inline sf::Vector2f vscale(const sf::Vector2f& a, float s) { return { a.x * s, a.y * s }; }
  inline float vlen(const sf::Vector2f& a) { return std::sqrt(a.x * a.x + a.y * a.y); }
  inline sf::Vector2f vnorm(const sf::Vector2f& a) {
    float L = vlen(a); if (L <= 1e-6f) return { 0.f,0.f }; return { a.x / L, a.y / L };
  }
}

void SteeringSystem::update(float dt) {
  for (auto& r : cfg.racers) {
    if (!r || !r->isSteeringEnabled()) continue;

    const float speed = r->getSpeed();
    const sf::Vector2f target = r->getTarget();
    auto tr = r->getComponent<Transform>();

    switch (r->getMode()) {
    case SteeringMode::Seek: {
      if (tr) {
        tr->seek(target, speed, dt, cfg.seekRange);
        r->setPosition(tr->getPosition());
      }
      break;
    }
    case SteeringMode::Arrive: {
      sf::Vector2f posL = r->getPosition();
      sf::Vector2f vel = arrive_L(posL, target, speed, r->getArriveRadius());
      posL = vadd(posL, vscale(vel, dt));
      r->setPosition(posL);
      if (tr) tr->setPosition(posL);
      break;
    }
    case SteeringMode::Pursuit: {
      sf::Vector2f posL = r->getPosition();
      if (auto t = r->getPursuitTarget().lock()) {
        sf::Vector2f vel = pursuit_L(*r, *t, speed, r->getMaxPrediction());
        posL = vadd(posL, vscale(vel, dt));
        r->setPosition(posL);
        if (tr) tr->setPosition(posL);
      }
      else if (tr) {
        tr->seek(target, speed, dt, cfg.seekRange);
        r->setPosition(tr->getPosition());
      }
      break;
    }
    }
  }
}

sf::Vector2f SteeringSystem::arrive_L(const sf::Vector2f& pos,
  const sf::Vector2f& target,
  float speed, float arriveRadius) {
  sf::Vector2f toT = vsub(target, pos);
  float d = vlen(toT);
  if (d < 1e-4f) return { 0.f, 0.f };
  float s = (d < arriveRadius) ? speed * (d / std::max(1e-4f, arriveRadius)) : speed;
  return vscale(vnorm(toT), s);
}

sf::Vector2f SteeringSystem::pursuit_L(const A_Racer& self,
  const A_Racer& target,
  float speed, float maxPrediction) {
  sf::Vector2f toT = vsub(target.getPosition(), self.getPosition());
  float d = vlen(toT);
  float pred = std::min(maxPrediction, d / std::max(1.f, target.getSpeed()));
  sf::Vector2f futDir = vnorm(toT);
  sf::Vector2f fut = vadd(target.getPosition(), vscale(futDir, target.getSpeed() * pred));

  sf::Vector2f aim = vsub(fut, self.getPosition());
  float L = vlen(aim);
  if (L < 1e-4f) return { 0.f, 0.f };
  return vscale(vnorm(aim), speed);
}