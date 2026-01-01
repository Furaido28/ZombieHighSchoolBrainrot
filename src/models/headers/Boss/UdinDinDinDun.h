#pragma once

#include "Boss.h"

#include "models/headers/Player.h"

class UdinDinDinDun : public Boss {

public:
    explicit UdinDinDinDun(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::Boss03; }

    int getDamage() const override;
    void attack(Player& player) override;

protected:
    void updatePhaseLogic(float dt, const sf::Vector2f& playerPos) override;

};
