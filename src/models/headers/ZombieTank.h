#pragma once

#include "ZombieBasic.h"

class ZombieTank : public ZombieBasic {
public:
    ZombieTank(const sf::Vector2f& startPos);

    ZombieType getType() const override { return ZombieType::Tank; }

    float getRadius() const override { return radius; }

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 100.f; }

    int getDamage() const override { return attackDamage; }
protected:
    float attackCooldown = 2.5;
    int attackDamage = 20;
};