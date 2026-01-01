#pragma once
#include "models/headers/Enemy.h"

class Boss : public Enemy {

public:
    explicit Boss(const sf::Vector2f& startPos);

    void update(float dt, const sf::Vector2f& playerPos) override;

protected:
    // ---- Phases ----
    int currentPhase = 0;
    float phaseTimer = 0.f;
    sf::Clock phaseClock;

    void updatePhase();

    // ---- Comportement générique ----
    void moveTowardPlayer(float dt, const sf::Vector2f& playerPos);

    virtual void updatePhaseLogic(float dt, const sf::Vector2f& playerPos) = 0;

};
