#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "../Components/Lap.h"
#include "ECS/A_Racer.h"
#include <vector>

struct RaceConfig {
    std::vector<EngineUtilities::TSharedPointer<Actor>> actors;
    const std::vector<sf::Vector2f>* waypoints = nullptr;
    float checkpointRadius = 12.f;
};

class RaceSystem {
public:
    RaceSystem(const RaceConfig& cfg);
    ~RaceSystem() = default;
    void update(float dt);
    std::vector<int> getStandings() const;
    const Lap& getLapData(size_t i) const { return laps[i]; }
    void setTimingActive(bool b) { timingActive = b; }
    float getPlayerCurrentLapTime() const { return playerLapTime; }
    float getPlayerBestLapTime() const { return (bestLapValid ? playerBestLap : -1.f); }
    bool hasBestLap() const { return bestLapValid; }

private:
    RaceConfig cfg;
    std::vector<Lap> laps;
    std::vector<float> progress;
    std::vector<float> elapsed;
    std::vector<float> lastS;
    int lapOwnerIndex = 0;
    float lapDebounceSec = 0.35f;
    float lapCooldown = 0.f;
    bool lapArmed = false;
    bool timingActive = false;
    float playerLapTime = 0.f;
    float playerBestLap = 0.f;
    bool bestLapValid = false;
    std::vector<float> prefix;
    float totalLen = 0.f;
    bool closedLoop = true;

    static sf::Vector2f getActorPos(const EngineUtilities::TSharedPointer<Actor>& a);
    float segProgress(const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b) const;
    float sAlongPath(const sf::Vector2f& p) const;
    void buildCircuitMeter();
    static float dist(const sf::Vector2f& A, const sf::Vector2f& B) {
        float dx = B.x - A.x, dy = B.y - A.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
