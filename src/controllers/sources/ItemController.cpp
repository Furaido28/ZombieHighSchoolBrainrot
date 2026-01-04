#include "controllers/headers/ItemController.h"
#include "controllers/headers/GameController.h"

ItemController::ItemController(
    WorldItemSystem& worldItems,
    GameController& game
)
    : worldItemSystem(worldItems), game(game) {}

void ItemController::tryPickup(Player& player) {
    int pickedIndex = -1;

    PickupResult result =
        worldItemSystem.tryPickup(player, pickedIndex);

    switch (result) {

        case PickupResult::LuckyBoxPicked:
            game.openLuckyBox(pickedIndex);
            break;

        case PickupResult::ItemPicked:
            // déjà ajouté à l’inventaire
            break;

        case PickupResult::None:
        default:
            break;
    }
}
