#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

class TralaleroTralala : public Boss {
public:
    explicit TralaleroTralala(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::Boss01; }

    int getDamage() const override;

protected:
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
