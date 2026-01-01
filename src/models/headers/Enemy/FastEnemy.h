#pragma once
#include "BasicEnemy.h"

class FastEnemy : public BasicEnemy {
public:
    explicit FastEnemy(const sf::Vector2f& startPos);
    EnemyType getType() const override { return EnemyType::Fast; }
    int getDamage() const;
};