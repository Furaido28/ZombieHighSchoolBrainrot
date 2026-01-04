#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

// Command to move the player downwards
class MoveDownCommand : public Command {
public:
    // Constructor: Links command to the player
    MoveDownCommand(Player& player) : player(player) {}

    // Executes the movement logic
    void execute(float dt) override {
        player.addMovement({0.f, 1.f});       // Add downward velocity
        player.setDirection(Direction::Down); // Face down
        player.setMoving(true);               // Mark as moving
    }

private:
    Player& player;

};