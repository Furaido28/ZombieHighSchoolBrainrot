#include "../headers/GameScene.h"

#include <cmath>
#include <iostream>

#include "../headers/MenuScene.h"
#include "../../core/headers/SceneManager.h"
#include "core/headers/AudioManager.h"
#include "scenes/headers/OptionsScene.h"
#include "views/headers/menu/OptionsMenu.h"

GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      inventoryView(controller.getPlayer().getInventory()),
      paused(false),
      pauseMenu(window->getSize().x, window->getSize().y)
{
    hudView.setSize(window->getSize().x, window->getSize().y);
    hudView.setCenter(
        window->getSize().x / 2.f,
        window->getSize().y / 2.f
    );

    if (!backgroundMusic.openFromFile("assets/sound/music/game.ogg")) {
        std::cerr << "Erreur mmusic/game.ogg\n";
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(
        AudioManager::getInstance().getFinalMusicVolume()
    );
    backgroundMusic.play();

    if (!font.loadFromFile("assets/fonts/font.ttf")) {
        std::cerr << "ERROR: FAILED TO LOAD UI FONT" << std::endl;
    }
}

void GameScene::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
    event.key.code == sf::Keyboard::Escape)
        {
        paused = !paused;

        if (paused) {
            pauseMenu.reset();
        }

        return;
        }

    // Si en pause → menu pause uniquement
    if (paused) {
        pauseMenu.handleEvent(event);

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

        if (pauseMenu.handleMouse(event, *window)) {
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

    controller.handleEvent(event);

    if (event.type == sf::Event::Resized) {
        hudView.setSize(event.size.width, event.size.height);
        hudView.setCenter(
            event.size.width / 2.f,
            event.size.height / 2.f
        );
    }
}


void GameScene::update(float dt) {
    if (!paused) {
        controller.update(dt);
        inventoryView.update(controller.isInventoryExpanded());
    }
}

void GameScene::render() {
    window->clear(sf::Color(20, 20, 20));

    // =====================
    // MONDE
    // =====================
    window->setView(controller.getGameView());
    controller.render(*window);

    // =====================
    // HUD
    // =====================
    window->setView(hudView);

    int wave = controller.getWaveNumber();
    float timeLeft = controller.getWaveTimeLeft();

    controller.getPlayerView().renderHUD(
        *window,
        controller.getPlayer(),
        wave,
        timeLeft
    );

    inventoryView.draw(*window);


    if (controller.isLevelEnding()) {
        sf::RectangleShape overlay;
        overlay.setSize({
            static_cast<float>(window->getSize().x),
            static_cast<float>(window->getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window->draw(overlay);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(72);
        text.setString("Next Level in 10 seconds\n");
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.width / 2.f, b.height / 2.f);
        text.setPosition(
            window->getSize().x / 2.f,
            window->getSize().y / 2.f
        );
        window->draw(text);
    }

    // =====================
    // PAUSE MENU
    // =====================
    if (paused) {
        sf::RectangleShape overlay;
        overlay.setSize({
            static_cast<float>(window->getSize().x),
            static_cast<float>(window->getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window->draw(overlay);

        pauseMenu.draw(*window);
    }

    window->display();
}