#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

// Boss implementation: UdinDinDinDun
class UdinDinDinDun : public Boss {
public:
    // Constructor: initializes the boss at a given position
    explicit UdinDinDinDun(const sf::Vector2f& pos);

    // Identifies this enemy as the third boss
    EnemyType getType() const override {
        return EnemyType::Boss03;
    }

    // Returns the damage dealt by this boss
    int getDamage() const override;

protected:
    // Creates the boss phase based on the current phase index
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
