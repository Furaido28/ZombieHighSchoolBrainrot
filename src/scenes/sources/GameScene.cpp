#include "../headers/GameScene.h"

#include <iostream>

#include "../headers/MenuScene.h"
#include "../../core/headers/SceneManager.h"

GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window), inventoryView(controller.getPlayer().getInventory()) {

    hudView.setSize(window->getSize().x, window->getSize().y);
    hudView.setCenter(
        window->getSize().x / 2.f,
        window->getSize().y / 2.f
    );

    if (!backgroundMusic.openFromFile("assets/sound/music/game.ogg")) {
        std::cerr << "Erreur mmusic/game.ogg\n";
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(15.f);
    backgroundMusic.play();
}

void GameScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);

    if (event.type == sf::Event::Resized) {
        hudView.setSize(event.size.width, event.size.height);
        hudView.setCenter(
            event.size.width / 2.f,
            event.size.height / 2.f
        );
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        manager->changeScene<MenuScene>(window);
    }
}


void GameScene::update(float dt) {
    controller.update(dt);
    inventoryView.update(controller.isInventoryExpanded());
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

    window->display();
}

