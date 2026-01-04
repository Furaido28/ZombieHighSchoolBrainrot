#include "scenes/headers/DefeatScene.h"
#include "scenes/headers/MenuScene.h"
#include "core/headers/SceneManager.h"

// Constructor: Initializes the menu with the window size
DefeatScene::DefeatScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y) {}

void DefeatScene::handleEvent(const sf::Event& event) {
    // Check if a button in the menu was clicked
    if (menu.handleEvent(event, *window)) {
        // Go back to the main menu
        manager->resetToMenu(window);
    }
}

void DefeatScene::render() {
    // Create a dark background (overlay)
    sf::RectangleShape overlay;

    // Set size to cover the whole window
    overlay.setSize({
        static_cast<float>(window->getSize().x),
        static_cast<float>(window->getSize().y)
    });

    // Set color to black with transparency
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // Draw the dark background first
    window->draw(overlay);

    // Draw the menu buttons on top
    menu.draw(*window);

    // Update the window display
    window->display();
}

