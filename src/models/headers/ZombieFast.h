#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H

#pragma once
#include "ZombieBasic.h"

class ZombieFast : public ZombieBasic {
public:
    ZombieFast(const sf::Vector2f& startPos);

    float getRadius() const override { return radius; }
    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 35; }
    ZombieType getType() const override {
        return ZombieType::Fast;
    }
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H