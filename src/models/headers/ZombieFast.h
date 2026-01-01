#pragma once

#include "ZombieBasic.h"

class ZombieFast : public ZombieBasic {
public:
    ZombieFast(const sf::Vector2f& startPos);

    ZombieType getType() const override { return ZombieType::Fast; }

    float getRadius() const override { return radius; }

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 35.f; }

    int getDamage() const override { return attackDamage; }
protected:
    float attackCooldown = 1.5;
    int attackDamage = 15;
};