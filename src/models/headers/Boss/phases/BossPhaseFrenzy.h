#pragma once
#include "BossPhase.h"
#include "models/headers/Boss/Boss.h"

// Frenzy phase: boss is extremely fast and very dangerous
class BossPhaseFrenzy : public BossPhase {
public:
    // Called once when the boss enters the frenzy phase
    void onEnter(Boss& boss) override {
        // Greatly increase boss movement speed
        boss.setMoveSpeed(160.f);
    }

    // Called every frame during this phase
    void update(Boss& boss, float dt, const sf::Vector2f& playerPos) override {
        // Boss aggressively chases the player
        boss.chasePlayer(dt, playerPos);
    }

    // Damage dealt by the boss in this phase
    int getDamage() const override {
        return 60;
    }
};
