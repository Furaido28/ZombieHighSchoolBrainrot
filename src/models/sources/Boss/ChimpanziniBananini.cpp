#include "../../headers/Boss/ChimpanziniBananini.h"
#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

// Constructor: initializes the specific boss stats
ChimpanziniBananini::ChimpanziniBananini(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 600.f;
    health = maxHealth;
    attackCooldown = 1.5f;
}

// Creates the appropriate boss phase based on phase index
std::unique_ptr<BossPhase>
ChimpanziniBananini::createPhase(int phaseIndex)
{
    switch (phaseIndex) {
        case 0: return std::make_unique<BossPhaseNormal>();
        case 1: return std::make_unique<BossPhaseEnraged>();
        default: return std::make_unique<BossPhaseFrenzy>();
    }
}

// Returns the damage value depending on the current phase
int ChimpanziniBananini::getDamage() const {
    return currentPhase ? currentPhase->getDamage() : 10;
}
