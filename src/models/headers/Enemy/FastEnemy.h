#pragma once

#include "BasicEnemy.h"
#include "models/headers/Enemy.h"

class FastEnemy : public BasicEnemy {
public:
    FastEnemy(const sf::Vector2f& startPos);

    EnemyType getType() const override { return EnemyType::Fast; }

    float getRadius() const override { return radius; }

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 35.f; }

    int getDamage() const override { return attackDamage; }
protected:
    float attackCooldown = 1.5;
    int attackDamage = 15;
};
