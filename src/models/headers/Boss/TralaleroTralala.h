#pragma once
#include "Boss.h"

class TralaleroTralala : public Boss {

public:
    explicit TralaleroTralala(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::Boss01; }

    int getDamage() const override;
    void attack(Player& player) override;

protected:
    void updatePhaseLogic(float dt, const sf::Vector2f& playerPos) override;

};
