#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

// Command to move the player upwards
class MoveUpCommand : public Command {
public:
    // Constructor: Links command to the player
    MoveUpCommand(Player& player) : player(player) {}

    // Executes the movement logic
    void execute(float dt) override {
        player.addMovement({0.f, -1.f});     // Add upward velocity
        player.setDirection(Direction::Up);  // Face up
        player.setMoving(true);              // Mark as moving
    }

private:
    Player& player;
};