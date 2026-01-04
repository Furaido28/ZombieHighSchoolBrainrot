#pragma once

/* ==========================================================
 * SFML Includes
 * ========================================================== */
#include <SFML/Audio.hpp>

/* ==========================================================
 * Standard Library Includes
 * ========================================================== */
#include <map>
#include <string>

/* ==========================================================
 * Project Includes
 * ========================================================== */
#include "ItemController.h"
#include "models/headers/Item.h"
#include "models/headers/Player.h"
#include "core/headers/LuckyBoxSystem.h"
#include "core/headers/items/WorldItemSystem.h"

/* ==========================================================
 * LuckyBoxController
 * Handles the opening of lucky boxes, item generation,
 * and screamer effects (visual + audio)
 * ========================================================== */
class LuckyBoxController {
public:
    /* =========================
     * Constructor
     * ========================= */

    // Creates a LuckyBoxController linked to the player,
    // the world item system, and the item controller
    LuckyBoxController(
        Player& player,
        WorldItemSystem& worldItemSystem,
        const ItemController& itemController
    );

    /* =========================
     * Lucky Box Logic
     * ========================= */

    // Opens a lucky box based on the given item index
    void openLuckyBox(int itemIndex);

    /* =========================
     * Screamer State
     * ========================= */

    // Returns true if the screamer effect is currently active
    bool isScreamerActive() const;

    // Updates screamer timing and state
    void update(float dt);

    // Renders the screamer visuals if active
    void render(sf::RenderWindow &window) const;

    // Triggers the screamer effect (sound + visual)
    void triggerScreamer();

private:
    /* ======================================================
     * Controllers & Systems
     * ====================================================== */

    // Reference to the item controller (used to manage items)
    const ItemController& itemController;

    // Reference to the player
    Player& player;

    // Reference to the world item system
    WorldItemSystem& worldItemSystem;

    /* ======================================================
     * Screamer State
     * ====================================================== */

    // Indicates whether the screamer is active
    bool screamerActive = false;

    // Current screamer timer
    float screamerTimer = 0.f;

    // Total duration of the screamer effect
    float screamerDuration = 0.6f;

    /* ======================================================
     * Screamer Visuals
     * ====================================================== */

    // Texture used for the screamer image
    sf::Texture screamerTexture;

    // Sprite used to display the screamer
    sf::Sprite screamerSprite;

    /* ======================================================
     * Screamer Audio
     * ====================================================== */

    // Sound buffer holding the screamer audio data
    sf::SoundBuffer screamerSoundBuffer;

    // Sound object used to play the screamer
    sf::Sound screamerSound;

    /* ======================================================
     * Item Creation Helpers
     * ====================================================== */

    // Creates a medkit item
    Item makeMedkit() const;

    // Creates a pen item
    Item makePen() const;

    // Creates a book item
    Item makeBook() const;

    // Creates a chalk item
    Item makeChalk() const;

    // Creates a laptop item
    Item makeLaptop() const;

    // Creates a deodorant item
    Item makeDeo() const;
};