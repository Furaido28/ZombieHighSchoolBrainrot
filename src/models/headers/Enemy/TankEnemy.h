#pragma once

#include "BasicEnemy"
#include "../Enemy.h"

class TankEnemy : public BasicEnemy {
public:
    TankEnemy(const sf::Vector2f& startPos);

    EnemyType getType() const override { return EnemyType::Tank; }

    float getRadius() const override { return radius; }

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 100.f; }

    int getDamage() const override { return attackDamage; }
protected:
    float attackCooldown = 2.5;
    int attackDamage = 20;
};
