#pragma once
#include "BossPhase.h"
#include "models/headers/Boss/Boss.h"

// Enraged phase: boss becomes faster and more aggressive
class BossPhaseEnraged : public BossPhase {
public:
    // Called once when the boss enters the enraged phase
    void onEnter(Boss& boss) override {
        // Increase boss movement speed
        boss.setMoveSpeed(90.f);
    }

    // Called every frame during this phase
    void update(Boss& boss, float dt, const sf::Vector2f& playerPos) override {
        // Boss actively chases the player
        boss.chasePlayer(dt, playerPos);
    }

    // Damage dealt by the boss in this phase
    int getDamage() const override {
        return 40;
    }
};
