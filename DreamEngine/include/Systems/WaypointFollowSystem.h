#pragma once
#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Prerequisites.h"
#include "ECS/A_Racer.h"

struct WaypointFollowConfig {
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;
    const std::vector<sf::Vector2f>* waypoints = nullptr;
    float arriveRadiusForAdvance = 12.f;
    float reactionDelay = 0.18f;
    float waypointNoiseRadius = 8.f;
    int   lookAhead = 1;
    float mistakeProb = 0.05f;
    float mistakeCooldown = 2.0f;
    float corridorWidth = 80.f;
    bool  cornerSlowdownEnabled = true;
    float cornerMinAngleDeg = 25.f;
    float cornerMaxAngleDeg = 95.f;
    float cornerMinFactor = 0.82f;
};

class WaypointFollowSystem {
public:
    WaypointFollowSystem(const WaypointFollowConfig& cfg)
        : cfg(cfg),
        indices(cfg.racers.size(), 0),
        reactTimers(cfg.racers.size(), 0.f),
        mistakeTimers(cfg.racers.size(), 0.f),
        rng(std::random_device{}()),
        uni(-1.f, 1.f),
        uni01(0.f, 1.f) {
    }

    ~WaypointFollowSystem() = default;

    void primeFromPositions() {
        if (!cfg.waypoints || cfg.waypoints->empty()) return;
        const auto& W = *cfg.waypoints;
        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) continue;
            sf::Vector2f p = r->getPosition();
            float best = std::numeric_limits<float>::max();
            size_t bestIdx = 0;
            for (size_t k = 0; k < W.size(); ++k) {
                float d = length(W[k] - p);
                if (d < best) { best = d; bestIdx = k; }
            }
            indices[i] = bestIdx;
            reactTimers[i] = 0.f;
            mistakeTimers[i] = 0.f;
            r->setTarget(W[bestIdx]);
        }
    }

    void update(float dt) {
        if (!cfg.waypoints || cfg.waypoints->empty()) return;
        const auto& W = *cfg.waypoints;
        if (W.empty()) return;

        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) continue;

            reactTimers[i] -= dt;
            if (reactTimers[i] > 0.f) continue;

            size_t idx = indices[i];
            size_t nextIdx = (idx + 1) % W.size();
            size_t nextNextIdx = (nextIdx + 1) % W.size();

            sf::Vector2f pos = r->getPosition();
            sf::Vector2f wp = W[idx];
            sf::Vector2f wpN = W[nextIdx];
            sf::Vector2f wpNN = W[nextNextIdx];

            sf::Vector2f seg = wpN - wp;
            float segLen2 = seg.x * seg.x + seg.y * seg.y;
            if (segLen2 <= 1e-4f) segLen2 = 1.f;

            int la = std::max(0, std::min(cfg.lookAhead, 2));
            size_t tgtIdx = (idx + static_cast<size_t>(la)) % W.size();
            sf::Vector2f target = W[tgtIdx];

            mistakeTimers[i] -= dt;
            if (mistakeTimers[i] <= 0.f && uni01(rng) < cfg.mistakeProb) {
                int sign = (uni01(rng) < 0.5f) ? -1 : 1;
                size_t wrong = (tgtIdx + W.size() + static_cast<size_t>(sign)) % W.size();
                target = W[wrong];
                mistakeTimers[i] = cfg.mistakeCooldown;
            }

            if (cfg.waypointNoiseRadius > 0.f) {
                target.x += uni(rng) * cfg.waypointNoiseRadius;
                target.y += uni(rng) * cfg.waypointNoiseRadius;
            }

            float d_to_wp = length(wp - pos);
            float d_to_target = length(target - pos);

            sf::Vector2f rel = pos - wp;
            float t = dot(rel, seg) / segLen2;
            float tClamped = std::max(0.f, std::min(1.f, t));

            sf::Vector2f proj = { wp.x + seg.x * tClamped, wp.y + seg.y * tClamped };
            float distToLine = length(proj - pos);

            bool crossedForward = (t > 0.6f) && (distToLine <= cfg.corridorWidth);
            bool shouldAdvance =
                (d_to_wp < cfg.arriveRadiusForAdvance) ||
                (d_to_target < (cfg.arriveRadiusForAdvance * 0.7f)) ||
                crossedForward;

            if (shouldAdvance) {
                indices[i] = (idx + 1) % W.size();
                reactTimers[i] = cfg.reactionDelay;
                size_t nidx = indices[i];
                size_t ntgt = (nidx + static_cast<size_t>(la)) % W.size();
                sf::Vector2f ntarget = W[ntgt];
                if (cfg.waypointNoiseRadius > 0.f) {
                    ntarget.x += uni(rng) * cfg.waypointNoiseRadius;
                    ntarget.y += uni(rng) * cfg.waypointNoiseRadius;
                }
                r->setTarget(ntarget);
            }
            else {
                r->setTarget(target);
            }

            if (cfg.cornerSlowdownEnabled) {
                sf::Vector2f v1 = normalize(wpN - wp);
                sf::Vector2f v2 = normalize(wpNN - wpN);
                float dotp = dot(v1, v2);
                dotp = std::max(-1.f, std::min(1.f, dotp));
                float angleRad = std::acos(dotp);
                float angleDeg = angleRad * 57.2957795f;

                float f = 1.f;
                if (angleDeg >= cfg.cornerMaxAngleDeg) {
                    f = cfg.cornerMinFactor;
                }
                else if (angleDeg <= cfg.cornerMinAngleDeg) {
                    f = 1.f;
                }
                else {
                    float tspan = (angleDeg - cfg.cornerMinAngleDeg) /
                        std::max(1.f, (cfg.cornerMaxAngleDeg - cfg.cornerMinAngleDeg));
                    f = 1.f + (cfg.cornerMinFactor - 1.f) * tspan;
                }

                float baseSpd = r->getSpeed();
                float limited = baseSpd * std::min(1.f, std::max(cfg.cornerMinFactor, f));
                if (limited < baseSpd) {
                    r->setSpeed(limited);
                }
            }
        }
    }

private:
    static float length(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    static sf::Vector2f normalize(const sf::Vector2f& v) {
        float len = length(v);
        if (len <= 1e-5f) return { 0.f, 0.f };
        return { v.x / len, v.y / len };
    }

    static float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
        return a.x * b.x + a.y * b.y;
    }

    WaypointFollowConfig cfg;
    std::vector<size_t> indices;
    std::vector<float>  reactTimers;
    std::vector<float>  mistakeTimers;
    std::mt19937 rng;
    std::uniform_real_distribution<float> uni;
    std::uniform_real_distribution<float> uni01;
};
