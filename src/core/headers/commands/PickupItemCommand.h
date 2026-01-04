#pragma once

#include "core/headers/Command.h"

class Player;
class WorldItemSystem;
class GameController;

// Command to handle the action of picking up items from the ground
class PickupItemCommand : public Command {
public:
    // Constructor
    PickupItemCommand(
        Player& player,
        WorldItemSystem& worldItemSystem,
        GameController& game
    );

    // Executes the logic: checks for nearby items and adds them to inventory
    void execute(float dt) override;

private:
    Player& player;                   // The entity picking up the item
    WorldItemSystem& worldItemSystem; // Manages items on the ground
    GameController& game;             // Reference to main game controller (for notifications/logic)
};