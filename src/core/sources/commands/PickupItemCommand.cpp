#include "core/headers/commands/PickupItemCommand.h"

#include <iostream>

#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"
#include "controllers/headers/GameController.h"

PickupItemCommand::PickupItemCommand(
    Player& p,
    WorldItemSystem& wis,
    GameController& g
) : player(p), worldItemSystem(wis), game(g) {
}

void PickupItemCommand::execute(float) {
    int index = -1;

    PickupResult result =
        worldItemSystem.tryPickup(player, index);

    if (result == PickupResult::LuckyBoxPicked) {
        game.openLuckyBox(index);
    }
    if (result == PickupResult::KeyFragment) {
        game.onKeyFragmentPicked();
        std::cout << "[DEBUG] KEY PICKED\n";
    }
}