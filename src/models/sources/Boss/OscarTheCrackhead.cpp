#include "models/headers/Boss/OscarTheCrackhead.h"

#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

// Constructor: initializes the boss with a position
OscarTheCrackhead::OscarTheCrackhead(const sf::Vector2f& pos)
    : Boss(pos) {
    // Set the maximum health of the boss
    maxHealth = 1200.f;

    // Current health starts at maximum
    health = maxHealth;

    //collision radius af the boss
    collisionRadius = 24.f;
    // Time (in seconds) between two attacks
    attackCooldown = 0.9f;
}

// Create the boss phase depending on the phase index
std::unique_ptr<BossPhase>
OscarTheCrackhead::createPhase(int phaseIndex) {
    switch (phaseIndex) {
        case 0:
            // First phase: normal behavior
            return std::make_unique<BossPhaseNormal>();

        case 1:
            // Second phase: enraged behavior
            return std::make_unique<BossPhaseEnraged>();

        case 2:
            // Third phase: frenzy behavior
            return std::make_unique<BossPhaseFrenzy>();

        default:
            // Safety fallback: always return a valid phase
            return std::make_unique<BossPhaseFrenzy>();
    }
}

// Returns the damage dealt by the boss
int OscarTheCrackhead::getDamage() const {
    // If a phase exists, add bonus damage to the phase damage
    // Otherwise, return a default damage value
    return currentPhase ? currentPhase->getDamage() + 15 : 40;
}
