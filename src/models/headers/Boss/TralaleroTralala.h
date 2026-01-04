#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

// Boss implementation: TralaleroTralala
class TralaleroTralala : public Boss {
public:
    // Constructor: initializes the boss at a given position
    explicit TralaleroTralala(const sf::Vector2f& pos);

    // Identifies this enemy as the first boss
    EnemyType getType() const override {
        return EnemyType::Boss01;
    }

    // Returns the damage dealt by this boss
    int getDamage() const override;

protected:
    // Creates the boss phase based on the current phase index
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
