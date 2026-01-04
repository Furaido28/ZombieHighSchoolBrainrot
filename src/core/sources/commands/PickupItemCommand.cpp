#include "core/headers/commands/PickupItemCommand.h"

#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"
#include "controllers/headers/GameController.h"

// Constructor: stores references to required systems
PickupItemCommand::PickupItemCommand(
    Player& p,
    WorldItemSystem& wis,
    GameController& g
) : player(p), worldItemSystem(wis), game(g) {
}

// Executes the pickup item command
void PickupItemCommand::execute(float) {
    int index = -1;

    // Try to pick up an item from the world
    PickupResult result =
        worldItemSystem.tryPickup(player, index);

    // If the picked item is a lucky box, trigger special behavior
    if (result == PickupResult::LuckyBoxPicked) {
        game.openLuckyBox(index);
    }
}
