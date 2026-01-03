#include "models/headers/Boss/UdinDinDinDun.h"

#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

UdinDinDinDun::UdinDinDinDun(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 900.f;
    health = maxHealth;
    attackCooldown = 2.0f;
    speed = 40.f;
}

std::unique_ptr<BossPhase>
UdinDinDinDun::createPhase(int phaseIndex)
{
    switch (phaseIndex) {
        case 0: return std::make_unique<BossPhaseNormal>();
        case 1: return std::make_unique<BossPhaseEnraged>();
        default: return std::make_unique<BossPhaseFrenzy>();
    }
}

int UdinDinDinDun::getDamage() const
{
    return currentPhase ? currentPhase->getDamage() + 10 : 30;
}

