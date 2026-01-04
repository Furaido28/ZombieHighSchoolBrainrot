#pragma once
#include "models/headers/Enemy.h"
#include <memory>
#include "phases/BossPhase.h"

// Base class for all bosses
class Boss : public Enemy {
public:
    // Constructor: initializes boss at a given position
    explicit Boss(const sf::Vector2f& startPos);

    // Updates boss behavior (movement + phase logic)
    void update(float dt, const sf::Vector2f& playerPos) override;

    // Sets the boss movement speed
    void setMoveSpeed(float s){ speed = s; }

    // Returns the current movement speed
    float getMoveSpeed(){ return speed; }

    // Helper method to chase the player
    void chasePlayer(float dt, const sf::Vector2f& playerPos) {
        moveTowardPlayer(dt, playerPos);
    }

protected:
    // Index of the current phase
    int currentPhaseIndex = -1;

    // Current active boss phase (state pattern)
    std::unique_ptr<BossPhase> currentPhase;

    // Updates the boss phase based on its state (health, logic, etc.)
    void updatePhase();

    // Changes the current phase
    void setPhase(std::unique_ptr<BossPhase> phase);

    // Each boss defines its own phases
    virtual std::unique_ptr<BossPhase> createPhase(int phaseIndex) = 0;

    // Common movement logic for all bosses
    void moveTowardPlayer(float dt, const sf::Vector2f& playerPos);

    // Ensures key fragment is dropped only once
    bool keyFragmentDropped = false;

    // Called when the boss dies
    virtual void onDeath();
};
