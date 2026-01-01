#pragma once
#include "models/headers/Enemy/BasicEnemy.h"

class TankEnemy : public BasicEnemy {
public:
    explicit TankEnemy(const sf::Vector2f& startPos);
    EnemyType getType() const override { return EnemyType::Tank; }
    int getDamage() const;
};
