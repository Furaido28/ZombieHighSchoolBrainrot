#include "../../headers/Boss/ChimpanziniBananini.h"
#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

ChimpanziniBananini::ChimpanziniBananini(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 600.f;
    health = maxHealth;
    attackCooldown = 1.5f;
}

std::unique_ptr<BossPhase>
ChimpanziniBananini::createPhase(int phaseIndex)
{
    switch (phaseIndex) {
        case 0: return std::make_unique<BossPhaseNormal>();
        case 1: return std::make_unique<BossPhaseEnraged>();
        default: return std::make_unique<BossPhaseFrenzy>();
    }
}

int ChimpanziniBananini::getDamage() const {
    return currentPhase ? currentPhase->getDamage() : 10;
}
