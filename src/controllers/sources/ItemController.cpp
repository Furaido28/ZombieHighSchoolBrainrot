#include "controllers/headers/ItemController.h"

#include <iostream>

#include "controllers/headers/GameController.h"

ItemController::ItemController(
    WorldItemSystem& worldItems,
    GameController& game
) : worldItemSystem(worldItems), game(game)
{
    loadTextures();
}

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
        case PickupResult::KeyFragment:
            game.onKeyFragmentPicked();
            std::cout<<"[DEBUG] KEY PICKED"<<std::endl;
            break;

        case PickupResult::None:
        default:
            break;
    }
}

void ItemController::loadTextures() {
    itemTextures["medkit"].loadFromFile("assets/inventory_items/medkit.png");
    itemTextures["pen"].loadFromFile("assets/inventory_items/pen.png");
    itemTextures["book"].loadFromFile("assets/inventory_items/book.png");
    itemTextures["chalk"].loadFromFile("assets/inventory_items/chalk.png");
    itemTextures["key-fragment"].loadFromFile("assets/inventory_items/key.png");
    itemTextures["laptop"].loadFromFile("assets/inventory_items/laptop.png");
    itemTextures["deo"].loadFromFile("assets/inventory_items/deo.png");
    itemTextures["luckybox"].loadFromFile("assets/inventory_items/luckybox.png");

    std::cout << "[DEBUG] Item textures loaded\n";
}

const sf::Texture& ItemController::getTexture(const std::string& name) const {
    return itemTextures.at(name);
}
