#include "models/headers/Boss/TralaleroTralala.h"

#include "models/headers/Boss/phases/BossPhaseNormal.h"
#include "models/headers/Boss/phases/BossPhaseEnraged.h"
#include "models/headers/Boss/phases/BossPhaseFrenzy.h"

TralaleroTralala::TralaleroTralala(const sf::Vector2f& pos)
    : Boss(pos) {
    maxHealth = 400.f;
    health = maxHealth;
    attackCooldown = 1.2f;
}

std::unique_ptr<BossPhase>
TralaleroTralala::createPhase(int phaseIndex) {
    switch (phaseIndex) {
        case 0: return std::make_unique<BossPhaseNormal>();
        case 1: return std::make_unique<BossPhaseEnraged>();
        default: return std::make_unique<BossPhaseFrenzy>();
    }
}

int TralaleroTralala::getDamage() const{
    return currentPhase ? currentPhase->getDamage() : 20;
}

