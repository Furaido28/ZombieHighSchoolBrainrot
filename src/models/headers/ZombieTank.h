#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIETANK_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIETANK_H

#pragma once
#include "ZombieBasic.h"

class ZombieTank : public ZombieBasic {
public:
    ZombieTank(const sf::Vector2f& startPos);

    ZombieType getType() const override {
        return ZombieType::Tank;
    }

    float getRadius() const override { return radius; }

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 100.f; }

};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIETANK_H