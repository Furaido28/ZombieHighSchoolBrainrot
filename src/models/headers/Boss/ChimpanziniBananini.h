#pragma once

#include "Boss.h"

#include "models/headers/Player.h"

class ChimpanziniBananini : public Boss {

public:
    explicit ChimpanziniBananini(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::Boss02; }

    int getDamage() const override;

protected:
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;

};
