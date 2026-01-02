#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

class OscarTheCrackhead : public Boss {
public:
    explicit OscarTheCrackhead(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::FinalBoss; }

    int getDamage() const override;

protected:
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
