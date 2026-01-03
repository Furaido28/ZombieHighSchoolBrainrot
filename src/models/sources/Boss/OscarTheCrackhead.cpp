#include "models/headers/Boss/OscarTheCrackhead.h"

#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

OscarTheCrackhead::OscarTheCrackhead(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 1200.f;
    health = maxHealth;
    attackCooldown = 0.9f;
}

std::unique_ptr<BossPhase>
OscarTheCrackhead::createPhase(int phaseIndex)
{
    switch (phaseIndex) {
        case 0: return std::make_unique<BossPhaseNormal>();
        case 1: return std::make_unique<BossPhaseEnraged>();
        case 2: return std::make_unique<BossPhaseFrenzy>();
        default: return std::make_unique<BossPhaseFrenzy>();
    }
}

int OscarTheCrackhead::getDamage() const
{
    // Final boss tape toujours plus fort
    return currentPhase ? currentPhase->getDamage() + 15 : 40;
}
