#include "../headers/MenuScene.h"

#include <iostream>

#include "../headers/GameScene.h"
#include "../../core/headers/SceneManager.h"
#include "core/headers/AudioManager.h"
#include "scenes/headers/OptionsScene.h"

MenuScene::MenuScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      controller(window->getSize().x, window->getSize().y) {

    if (!backgroundMusic.openFromFile("assets/sound/music/menu.ogg")) {
        std::cerr << "Erreur music/menu.ogg\n";
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(
        AudioManager::getInstance().getFinalMusicVolume()
    );
    backgroundMusic.play();
}

void MenuScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);

    int c = controller.getSelectedChoice();

    if (c == 0) {
        backgroundMusic.stop();
        manager->changeScene<GameScene>(window);
    }

    if (c == 1)
        manager->changeScene<OptionsScene>(window);

    if (c == 2) {
        backgroundMusic.stop();
        window->close();
    }
}

void MenuScene::update(float dt) {
    controller.update(dt);
}

void MenuScene::render() {
    window->clear();
    controller.render(*window);
    window->display();
}
