#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H

#pragma once
#include "ZombieBasic.h"

class ZombieFast : public ZombieBasic {
public:
    ZombieFast(const sf::Vector2f& startPos);

    ZombieType getType() const override {
        return ZombieType::Fast;
    }
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEFAST_H