#pragma once
#include "models/headers/Enemy.h"
#include <memory>
#include "phases/BossPhase.h"

class Boss : public Enemy {
public:
    explicit Boss(const sf::Vector2f& startPos);

    void update(float dt, const sf::Vector2f& playerPos) override;
    void setMoveSpeed(float s){speed = s;}
    float getMoveSpeed(){return speed;}
    void chasePlayer(float dt, const sf::Vector2f& playerPos) {
        moveTowardPlayer(dt, playerPos);
    }

protected:
    int currentPhaseIndex = -1;
    std::unique_ptr<BossPhase> currentPhase;

    void updatePhase();
    void setPhase(std::unique_ptr<BossPhase> phase);

    // Chaque boss choisit ses phases
    virtual std::unique_ptr<BossPhase> createPhase(int phaseIndex) = 0;

    // Mouvement commun
    void moveTowardPlayer(float dt, const sf::Vector2f& playerPos);
};

