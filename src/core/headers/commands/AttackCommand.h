#pragma once

#include "../Command.h"
#include "models/headers/Player.h"

// Command that triggers the player's attack
class AttackCommand : public Command {
public:
    // Constructor: Links this command to the specific player
    explicit AttackCommand(Player& player)
        : player(player) {}

    // Executes the attack action (called when the button is pressed)
    void execute(float dt) override;

private:
    Player& player; // Reference to the player being controlled
};