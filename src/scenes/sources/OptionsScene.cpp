#include "scenes/headers/OptionsScene.h"
#include "scenes/headers/MenuScene.h"

#include "core/headers/AudioManager.h"
#include "core/headers/SceneManager.h"
#include "scenes/headers/GameScene.h"

// Constructor: Initializes the menu and saves the return target
OptionsScene::OptionsScene(SceneManager* manager,
                           sf::RenderWindow* window,
                           OptionsReturnTarget target)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y),
      returnTarget(target) {}

void OptionsScene::handleEvent(const sf::Event& event) {

    // Handle Keyboard Inputs
    if (event.type == sf::Event::KeyPressed) {
        menu.handleInput(event.key.code); // Move selection up/down

        // If Enter is pressed on the "Back" button
        if (event.key.code == sf::Keyboard::Enter &&
            menu.getSelectedIndex() == 3) {

            // Return to the correct previous scene
            if (returnTarget == OptionsReturnTarget::Menu)
                manager->resetToMenu(window);
            else if (returnTarget == OptionsReturnTarget::Game)
                manager->popScene(); // Remove options scene to go back to game
            }
    }

    // Handle Mouse Inputs (Hovering)
    menu.handleMouse(event, *window);

    // Check if "Back" button was clicked
    if (menu.buttonBackClicked(event, *window)) {
        if (returnTarget == OptionsReturnTarget::Menu) {
            manager->resetToMenu(window);
        }
        else if (returnTarget == OptionsReturnTarget::Game) {
            manager->popScene(); // Resume game
        }
    }
}

void OptionsScene::update(float) {}

void OptionsScene::render() {
    window->clear(sf::Color::Black); // Clear background
    menu.draw(*window); // Draw the options menu
    window->display();
}
