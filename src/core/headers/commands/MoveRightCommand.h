#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

// Command to move the player to the right
class MoveRightCommand : public Command {
public:
    // Constructor: Links command to the player
    MoveRightCommand(Player& player) : player(player) {}

    // Executes the movement logic
    void execute(float dt) override {
        player.addMovement({1.f, 0.f});       // Add rightward velocity
        player.setDirection(Direction::Right); // Face right
        player.setMoving(true);               // Mark as moving

    }

private:
    Player& player;

};