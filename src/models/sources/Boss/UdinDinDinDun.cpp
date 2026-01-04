#include "models/headers/Boss/UdinDinDinDun.h"

#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

// Constructor: initializes the boss with a position
UdinDinDinDun::UdinDinDinDun(const sf::Vector2f& pos)
    : Boss(pos) {
    // Maximum health of the boss
    maxHealth = 900.f;

    // Boss starts with full health
    health = maxHealth;
    collisionRadius = 24.f;
    // Slow attack speed (long cooldown)
    attackCooldown = 2.0f;

    // Movement speed of the boss
    speed = 40.f;
}

// Creates the boss phase based on the current phase index
std::unique_ptr<BossPhase>
UdinDinDinDun::createPhase(int phaseIndex) {
    switch (phaseIndex) {
        case 0:
            // First phase: normal behavior
            return std::make_unique<BossPhaseNormal>();

        case 1:
            // Second phase: enraged behavior
            return std::make_unique<BossPhaseEnraged>();

        default:
            // Fallback phase: frenzy behavior
            return std::make_unique<BossPhaseFrenzy>();
    }
}

// Returns the damage dealt by the boss
int UdinDinDinDun::getDamage() const {
    // Add a small damage bonus if a phase is active
    // Otherwise, return a default damage value
    return currentPhase ? currentPhase->getDamage() + 10 : 30;
}
