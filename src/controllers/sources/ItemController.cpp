/* ==========================================================
 * Includes
 * ========================================================== */
#include "controllers/headers/ItemController.h"

#include <iostream>

#include "controllers/headers/GameController.h"

/* ==========================================================
 * ItemController Constructor
 * Initializes item systems and loads textures
 * ========================================================== */
ItemController::ItemController(
    WorldItemSystem& worldItems,
    GameController& game
) : worldItemSystem(worldItems), game(game)
{
    // Load all item textures at initialization
    loadTextures();
}

/* ==========================================================
 * ItemController::tryPickup
 * Attempts to pick up an item from the world
 * ========================================================== */
void ItemController::tryPickup(Player& player) {

    // Index of the picked item (if any)
    int pickedIndex = -1;

    // Attempt to pick up an item via the world item system
    PickupResult result =
        worldItemSystem.tryPickup(player, pickedIndex);

    /* =========================
     * Pickup Result Handling
     * ========================= */
    switch (result) {

        case PickupResult::LuckyBoxPicked:
            // Open lucky box with the picked item index
            game.openLuckyBox(pickedIndex);
            break;

        case PickupResult::ItemPicked:
            // Item already added to inventory
            break;

        case PickupResult::KeyFragment:
            // Notify game that a key fragment was picked
            game.onKeyFragmentPicked();
            std::cout << "[DEBUG] KEY PICKED" << std::endl;
            break;

        case PickupResult::None:
        default:
            // Nothing picked
            break;
    }
}

/* ==========================================================
 * ItemController::loadTextures
 * Loads all inventory item textures
 * ========================================================== */
void ItemController::loadTextures() {

    // Inventory item textures
    itemTextures["medkit"].loadFromFile("assets/inventory_items/medkit.png");
    itemTextures["pen"].loadFromFile("assets/inventory_items/pen.png");
    itemTextures["book"].loadFromFile("assets/inventory_items/book.png");
    itemTextures["chalk"].loadFromFile("assets/inventory_items/chalk.png");
    itemTextures["key-fragment"].loadFromFile("assets/inventory_items/key.png");
    itemTextures["laptop"].loadFromFile("assets/inventory_items/laptop.png");
    itemTextures["deo"].loadFromFile("assets/inventory_items/deo.png");
    itemTextures["luckybox"].loadFromFile("assets/inventory_items/luckybox.png");

    // Debug output
    std::cout << "[DEBUG] Item textures loaded\n";
}

/* ==========================================================
 * ItemController::getTexture
 * Returns a texture by item name
 * ========================================================== */
const sf::Texture& ItemController::getTexture(const std::string& name) const {

    // Retrieve texture from the map (throws if not found)
    return itemTextures.at(name);
}