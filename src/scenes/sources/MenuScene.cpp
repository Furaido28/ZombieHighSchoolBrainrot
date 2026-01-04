#include "../headers/MenuScene.h"

#include <iostream>

#include "../headers/GameScene.h"
#include "../../core/headers/SceneManager.h"
#include "core/headers/AudioManager.h"
#include "scenes/headers/OptionsScene.h"

// Constructor: Setup the menu and load music
MenuScene::MenuScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      controller(window->getSize().x, window->getSize().y) { // Initialize controller with window size

    // Load the music file
    if (!backgroundMusic.openFromFile("assets/sound/music/menu.ogg")) {
        std::cerr << "Erreur music/menu.ogg\n";
    }

    // Configure and play music
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(
        AudioManager::getInstance().getFinalMusicVolume() // Get saved volume settings
    );
    backgroundMusic.play();
}

void MenuScene::handleEvent(const sf::Event& event) {
    // Pass the input to the controller (to handle button hovering/clicking)
    controller.handleEvent(event);

    // Get the ID of the button that was clicked
    int c = controller.getSelectedChoice();

    // Choice 0: Play Game
    if (c == 0) {
        backgroundMusic.stop(); // Stop menu music
        manager->pushScene<GameScene>(window); // Switch to GameScene
    }

    // Choice 1: Options
    if (c == 1) manager->pushScene<OptionsScene>(
        window,
        OptionsReturnTarget::Menu // Return to Menu after options
    );

    // Choice 2: Exit Game
    if (c == 2) {
        backgroundMusic.stop();
        window->close(); // Close the program
    }
}

void MenuScene::update(float dt) {
    // Update menu animations or effects
    controller.update(dt);
}

void MenuScene::render() {
    window->clear(); // Clear the screen
    controller.render(*window); // Draw the menu UI
    window->display(); // Show the frame
}
