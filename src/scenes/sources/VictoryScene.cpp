#include "scenes/headers/VictoryScene.h"
#include "scenes/headers/MenuScene.h"
#include "core/headers/SceneManager.h"

// Constructor: Setup the menu with window size
VictoryScene::VictoryScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y) {}

void VictoryScene::handleEvent(const sf::Event& event) {
    // Check if user clicked "Return to Menu" button
    if (menu.handleEvent(event, *window)) {
        manager->resetToMenu(window); // Go back to main menu
    }
}

void VictoryScene::render() {
    // Create a dark overlay background
    sf::RectangleShape overlay;
    overlay.setSize({
        static_cast<float>(window->getSize().x),
        static_cast<float>(window->getSize().y)
    });
    // Black color with transparency
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // Draw the background
    window->draw(overlay);

    // Draw the victory menu on top
    menu.draw(*window);

    // Display the frame
    window->display();
}
