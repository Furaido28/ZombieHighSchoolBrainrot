#include "../headers/GameScene.h"

#include <cmath>
#include <iostream>

#include "../headers/MenuScene.h"
#include "../../core/headers/SceneManager.h"
#include "core/headers/AudioManager.h"
#include "scenes/headers/DefeatScene.h"
#include "scenes/headers/OptionsScene.h"
#include "scenes/headers/VictoryScene.h"
#include "views/headers/menu/OptionsMenu.h"

// Constructor: Initializes the game state
GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      inventoryView(controller.getPlayer().getInventory()),
      paused(false),
      pauseMenu(window->getSize().x, window->getSize().y)
{
    // Setup the static camera for the UI (HUD)
    hudView.setSize(window->getSize().x, window->getSize().y);
    hudView.setCenter(
        window->getSize().x / 2.f,
        window->getSize().y / 2.f
    );

    // Load background music
    if (!backgroundMusic.openFromFile("assets/sound/music/game.ogg")) {
        std::cerr << "Erreur mmusic/game.ogg\n";
    }

    // Configure and play music
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(
        AudioManager::getInstance().getFinalMusicVolume()
    );
    backgroundMusic.play();

    // Load font for UI text
    if (!font.loadFromFile("assets/fonts/font.ttf")) {
        std::cerr << "ERROR: FAILED TO LOAD UI FONT" << std::endl;
    }
}

void GameScene::handleEvent(const sf::Event& event) {
    // Toggle pause when Escape is pressed
    if (event.type == sf::Event::KeyPressed &&
    event.key.code == sf::Keyboard::Escape)
        {
        paused = !paused;

        if (paused) {
            pauseMenu.reset(); // Reset menu selection
        }

        return;
        }

    // If paused, only menu pause
    if (paused) {
        pauseMenu.handleEvent(event);

        // Handle mouse clicks on the pause menu
        if (pauseMenu.handleMouse(event, *window)) {
            int choice = pauseMenu.getSelectedIndex();

            if (choice == 0) paused = false; // Resume Game
            else if (choice == 1)
                manager->pushScene<OptionsScene>( // Open Options
                    window,
                    OptionsReturnTarget::Game
                );
            else if (choice == 2)
                manager->resetToMenu(window); // Quit to Main Menu
        }

        // Handle keyboard (Enter key) on the pause menu
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter) {
            int choice = pauseMenu.getSelectedIndex();

            if (choice == 0) paused = false;
            else if (choice == 1)
                manager->pushScene<OptionsScene>(
                    window,
                    OptionsReturnTarget::Game
                );
            else if (choice == 2)
                manager->resetToMenu(window);
        }

        return;
    }

    // If not paused, handle standard game inputs (shooting, moving)
    controller.handleEvent(event);

    // Adjust UI camera if the window is resized
    if (event.type == sf::Event::Resized) {
        hudView.setSize(event.size.width, event.size.height);
        hudView.setCenter(
            event.size.width / 2.f,
            event.size.height / 2.f
        );
    }
}


void GameScene::update(float dt) {
    // Check if player died -> Go to Defeat Screen
    if (controller.isPlayerDead()) {
        manager->pushScene<DefeatScene>(window);
        return;
    }

    // Update game logic only if not paused
    if (!paused) {
        controller.update(dt);
        inventoryView.update(controller.isInventoryExpanded());
    }
}

void GameScene::render() {
    // Clear screen with dark gray color
    window->clear(sf::Color(20, 20, 20));

    // =====================
    // WORLD RENDER
    // =====================
    // Set camera to follow the player
    window->setView(controller.getGameView());
    controller.render(*window);

    // =====================
    // HUD RENDER
    // =====================
    // Set camera to static UI view
    window->setView(hudView);

    int wave = controller.getWaveNumber();
    float timeLeft = controller.getWaveTimeLeft();

    // Draw player health and wave info
    controller.getPlayerView().renderHUD(
        *window,
        controller.getPlayer(),
        wave,
        timeLeft
    );

    // Draw inventory slots
    inventoryView.draw(*window);


    // If level is ending, draw a transition message
    if (controller.isLevelEnding()) {
        sf::RectangleShape overlay;
        overlay.setSize({
            static_cast<float>(window->getSize().x),
            static_cast<float>(window->getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160)); // Dark overlay
        window->draw(overlay);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(72);
        text.setString("Next Level in 10 seconds\n");
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        // Center the text
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.width / 2.f, b.height / 2.f);
        text.setPosition(
            window->getSize().x / 2.f,
            window->getSize().y / 2.f
        );
        window->draw(text);
    }

    // =====================
    // PAUSE MENU RENDER
    // =====================
    if (paused) {
        // Draw dark background behind menu
        sf::RectangleShape overlay;
        overlay.setSize({
            static_cast<float>(window->getSize().x),
            static_cast<float>(window->getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window->draw(overlay);

        // Draw menu buttons
        pauseMenu.draw(*window);
    }

    // Display final frame
    window->display();
}