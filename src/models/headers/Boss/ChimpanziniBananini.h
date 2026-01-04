#pragma once

#include "Boss.h"
#include "models/headers/Player.h"

// Specific boss implementation: ChimpanziniBananini
class ChimpanziniBananini : public Boss {

public:
    // Constructor: initializes the boss at a given position
    explicit ChimpanziniBananini(const sf::Vector2f& pos);

    // Returns the enemy type used to identify this boss
    EnemyType getType() const override {
        return EnemyType::Boss02;
    }

    // Returns the damage dealt by this boss
    int getDamage() const override;

protected:
    // Creates the boss phase based on the current phase index
    std::unique_ptr<BossPhase> createPhase(int phaseIndex) override;
};
