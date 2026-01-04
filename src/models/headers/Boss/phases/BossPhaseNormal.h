#pragma once
#include "BossPhase.h"
#include "models/headers/Boss/Boss.h"

// Normal phase: default boss behavior
class BossPhaseNormal : public BossPhase {
public:
    // Called once when the boss enters the normal phase
    void onEnter(Boss& boss) override {
        // Set standard movement speed
        boss.setMoveSpeed(50.f);
    }

    // Called every frame during this phase
    void update(Boss& boss, float dt, const sf::Vector2f& playerPos) override {
        // Boss moves toward the player
        boss.chasePlayer(dt, playerPos);
    }

    // Damage dealt by the boss in this phase
    int getDamage() const override {
        return 25;
    }
};
