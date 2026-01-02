#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

class UdinDinDinDun : public Boss {
public:
    explicit UdinDinDinDun(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::Boss03; }

    int getDamage() const override;

protected:
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
