#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/A_Racer.h"
#include "ECS/Transform.h"

struct SteeringConfig {
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;
    float seekRange = 10.f;
};

class SteeringSystem {
public:
    SteeringSystem(const SteeringConfig& cfg) : cfg(cfg) {}
    ~SteeringSystem() = default;
    void update(float dt);

private:
    SteeringConfig cfg;

    sf::Vector2f arrive_L(const sf::Vector2f& pos,
        const sf::Vector2f& target,
        float speed, float arriveRadius);

    sf::Vector2f pursuit_L(const A_Racer& self,
        const A_Racer& target,
        float speed, float maxPrediction);
};
