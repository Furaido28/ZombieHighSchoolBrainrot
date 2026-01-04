#pragma once

#include "../Command.h"
#include "models/headers/Player.h"

// Command to use the currently selected item
class UseItemCommand : public Command {
public:
    // Constructor: Links command to the player
    explicit UseItemCommand(Player& player)
        : player(player) {}

    // Executes the action
    void execute(float dt) override;

private:
    Player& player; // Reference to the player

};