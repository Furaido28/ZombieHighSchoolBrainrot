#pragma once

#include "Boss.h"

class OscarTheCrackhead : public Boss {
public:
    explicit OscarTheCrackhead(const sf::Vector2f& startPos);

    int getDamage() const override { return 30; }
    float getMaxHealth() const override { return 1200.f; }

    void update(float dt, const sf::Vector2f& playerPos) override;
};
