/* ==========================================================
 * Includes
 * ========================================================== */
#include "../headers/MenuController.h"

/* ==========================================================
 * MenuController Constructor
 * Initializes the menu and default choice state
 * ========================================================== */
MenuController::MenuController(float width, float height)
: menu(width, height), choice(-1) {}

/* ==========================================================
 * MenuController::handleEvent
 * Handles keyboard and mouse events for the menu
 * ========================================================== */
void MenuController::handleEvent(const sf::Event& event) {

    /* =========================
     * Keyboard Navigation
     * ========================= */
    if (event.type == sf::Event::KeyPressed) {

        // Navigate up in the menu
        if (event.key.code == sf::Keyboard::Up)
            menu.moveUp();

        // Navigate down in the menu
        if (event.key.code == sf::Keyboard::Down)
            menu.moveDown();

        // Validate selection
        if (event.key.code == sf::Keyboard::Enter)
            choice = menu.getSelectedIndex();
    }

    /* =========================
     * Mouse Hover
     * ========================= */
    if (event.type == sf::Event::MouseMoved) {

        // Update hovered menu item based on mouse position
        menu.hoverWithMouse({
            float(event.mouseMove.x),
            float(event.mouseMove.y)
        });
    }

    /* =========================
     * Mouse Click Selection
     * ========================= */
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        // Mouse click position
        sf::Vector2f p(
            event.mouseButton.x,
            event.mouseButton.y
        );

        // Check each menu item
        for (int i = 0; i < 3; i++)
            if (menu.isMouseOnItem(p, i))
                choice = i;
    }
}

/* ==========================================================
 * MenuController::update
 * Updates menu state (unused here)
 * ========================================================== */
void MenuController::update(float dt) {
    // No dynamic update needed
}

/* ==========================================================
 * MenuController::render
 * Renders the menu to the window
 * ========================================================== */
void MenuController::render(sf::RenderWindow& window) {
    menu.draw(window);
}

/* ==========================================================
 * MenuController::getSelectedChoice
 * Returns the selected menu option
 * ========================================================== */
int MenuController::getSelectedChoice() const {
    return choice;
}
