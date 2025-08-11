#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "./Actor.h"
#include "./Transform.h"

// Modo de control del jugador
enum class PlayerControlMode {
    Direct,
    TargetSeek
};

class A_Player : public Actor {
public:
    A_Player(const std::string& name) : Actor(name) {}

    // Control mode
    void setControlMode(PlayerControlMode m) { controlMode = m; }
    PlayerControlMode getControlMode() const { return controlMode; }

    // Parámetros para control directo (WASD)
    void  setAcceleration(float a) { acceleration = a; }
    float getAcceleration() const { return acceleration; }

    void  setFriction(float f) { friction = f; }
    float getFriction() const { return friction; }

    void  setMaxSpeed(float s) { maxSpeed = s; }
    float getMaxSpeed() const { return maxSpeed; }

    void           setVelocity(const sf::Vector2f& v) { velocity = v; }
    sf::Vector2f   getVelocity() const { return velocity; }

    // Posición/target usando Transform como fuente de verdad del player
    sf::Vector2f getPosition() {
        auto tr = getComponent<Transform>();
        if (tr) return tr->getPosition();
        return { 0.f, 0.f };
    }

    void setPosition(const sf::Vector2f& p) {
        if (auto tr = getComponent<Transform>()) tr->setPosition(p);
    }

    void           setTarget(const sf::Vector2f& t) { target = t; }
    sf::Vector2f   getTarget() const { return target; }

private:
    PlayerControlMode controlMode = PlayerControlMode::Direct;
    sf::Vector2f      velocity{ 0.f, 0.f };
    float             acceleration = 600.f; // px/s^2
    float             friction = 6.f;
    float             maxSpeed = 250.f;
    sf::Vector2f      target{ 0.f, 0.f };
};
