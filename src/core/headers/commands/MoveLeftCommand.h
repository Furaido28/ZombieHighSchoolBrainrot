#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

// Command to move the player to the left
class MoveLeftCommand : public Command {
public:
    // Constructor: Links command to the player
    MoveLeftCommand(Player& player) : player(player) {}

    // Executes the movement logic
    void execute(float dt) override {
        player.addMovement({-1.f, 0.f});      // Add leftward velocity
        player.setDirection(Direction::Left); // Face left
        player.setMoving(true);               // Mark as moving
    }

private:
    Player& player;

};