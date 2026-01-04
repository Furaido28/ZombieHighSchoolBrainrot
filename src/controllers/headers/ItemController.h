#pragma once

/* ==========================================================
 * Models & Systems
 * ========================================================== */
#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"

/* ==========================================================
 * Forward Declarations
 * ========================================================== */
class GameController;

/* ==========================================================
 * ItemController
 * Manages item pickup, item textures, and world item logic
 * ========================================================== */
class ItemController {
public:
    /* =========================
     * Constructor
     * ========================= */
    ItemController(
        WorldItemSystem& worldItems,
        GameController& game
    );

    /* =========================
     * Item Interaction
     * ========================= */

    // Attempts to pick up nearby items and add them to the player inventory
    void tryPickup(Player& player);

    /* =========================
     * Texture Access
     * ========================= */

    // Returns the texture associated with an item name
    const sf::Texture& getTexture(const std::string& name) const;

private:
    /* ======================================================
     * References to Game Systems
     * ====================================================== */
    WorldItemSystem& worldItemSystem;
    GameController& game;

    /* ======================================================
     * Item Assets
     * ====================================================== */
    std::map<std::string, sf::Texture> itemTextures;

    /* ======================================================
     * Internal Helpers
     * ====================================================== */

    // Loads all item textures at startup
    void loadTextures();
};
