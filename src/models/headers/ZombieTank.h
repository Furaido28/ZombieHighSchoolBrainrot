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
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIETANK_H