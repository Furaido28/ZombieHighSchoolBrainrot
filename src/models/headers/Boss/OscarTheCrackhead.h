#pragma once

#include "Boss.h"
#include "models/headers/Player.h"
#include <memory>

// Final boss implementation
class OscarTheCrackhead : public Boss {
public:
    // Constructor: initializes the boss at a given position
    explicit OscarTheCrackhead(const sf::Vector2f& pos);

    // Identifies this enemy as the final boss
    EnemyType getType() const override {
        return EnemyType::FinalBoss;
    }

    // Returns the damage dealt by the boss
    int getDamage() const override;

protected:
    // Creates the boss phase based on the current phase index
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
