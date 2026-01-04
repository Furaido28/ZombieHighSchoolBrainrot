#include "core/headers/commands/PickupItemCommand.h"

#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"
#include "controllers/headers/GameController.h"

PickupItemCommand::PickupItemCommand(
    Player& p,
    WorldItemSystem& wis,
    GameController& g
) : player(p), worldItemSystem(wis), game(g) {}

void PickupItemCommand::execute(float) {
    worldItemSystem.tryPickup(player, game);
}
