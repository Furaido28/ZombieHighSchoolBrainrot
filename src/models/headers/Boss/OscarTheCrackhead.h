#pragma once

#include "Boss.h"

#include "models/headers/Player.h"

class OscarTheCrackhead : public Boss {

public:
    explicit OscarTheCrackhead(const sf::Vector2f& pos);

    EnemyType getType() const override { return EnemyType::FinalBoss; }

    int getDamage() const override;
    void attack(Player& player) override;

protected:
    void updatePhaseLogic(float dt, const sf::Vector2f& playerPos) override;

};
