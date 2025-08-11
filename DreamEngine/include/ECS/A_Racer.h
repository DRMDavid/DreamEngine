#pragma once
#include "Actor.h"
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"

// Modos de steering
enum class SteeringMode { Seek, Arrive, Pursuit };

class A_Racer : public Actor {
public:
    A_Racer(const std::string& name);

    void update(float deltaTime) override;

    void setPlace(int newPlace) { place = newPlace; }
    int  getPlace() const { return place; }

    // Estado lógico de movimiento
    void           setPosition(const sf::Vector2f& pos) { logicPosition = pos; }
    sf::Vector2f   getPosition() const { return logicPosition; }

    void           setTarget(const sf::Vector2f& waypoint) { logicTarget = waypoint; }
    sf::Vector2f   getTarget() const { return logicTarget; }

    void  setSpeed(float s) { speed = s; }
    float getSpeed() const { return speed; }

    // Steering flags & params
    void enableSteering(bool enabled) { steeringEnabled = enabled; }
    bool isSteeringEnabled() const { return steeringEnabled; }

    void  setArriveRadius(float radius) { arriveRadius = radius; }
    float getArriveRadius() const { return arriveRadius; }

    void         setMode(SteeringMode m) { mode = m; }
    SteeringMode getMode() const { return mode; }

    // Pursuit
    void setPursuitTarget(const EngineUtilities::TWeakPointer<A_Racer>& t) { pursuitTarget = t; }
    EngineUtilities::TWeakPointer<A_Racer> getPursuitTarget() const { return pursuitTarget; }

    void  setMaxPrediction(float p) { maxPrediction = p; }
    float getMaxPrediction() const { return maxPrediction; }

private:
    int         place = 0;

    sf::Vector2f logicPosition{ 0.f, 0.f };
    sf::Vector2f logicTarget{ 0.f, 0.f };
    float        speed = 100.f;

    bool         steeringEnabled = true;
    float        arriveRadius = 10.f;
    SteeringMode mode = SteeringMode::Seek;

    EngineUtilities::TWeakPointer<A_Racer> pursuitTarget;
    float        maxPrediction = 0.5f; // segundos
};
