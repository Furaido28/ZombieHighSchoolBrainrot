#pragma once

/* ==========================================================
 * SFML Includes
 * ========================================================== */
#include <SFML/Graphics.hpp>

/* ==========================================================
 * Views
 * ========================================================== */
#include "../../views/headers/menu/MenuView.h"

/* ==========================================================
 * MenuController
 * Handles menu navigation, selection, and rendering
 * ========================================================== */
class MenuController {
public:
    /* =========================
     * Constructor
     * ========================= */

    // Creates the menu with the given window dimensions
    MenuController(float width, float height);

    /* =========================
     * Menu Lifecycle
     * ========================= */

    // Handles discrete input events (keyboard, mouse, etc.)
    void handleEvent(const sf::Event& event);

    // Updates menu state (animations, selection timing, etc.)
    void update(float dt);

    // Renders the menu to the window
    void render(sf::RenderWindow& window);

    /* =========================
     * Selection
     * ========================= */

    // Returns the currently selected menu option
    int getSelectedChoice() const;

private:
    /* ======================================================
     * Menu View
     * ====================================================== */
    Menu menu;

    /* ======================================================
     * Internal State
     * ====================================================== */
    int choice = 0;  // Current selected menu index
};
