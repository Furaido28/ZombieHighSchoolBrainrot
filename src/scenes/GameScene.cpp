#include "GameScene.h"
#include "MenuScene.h"
#include "core/SceneManager.h"

GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
: Scene(manager, window) {}

void GameScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
        manager->changeScene<MenuScene>(window);
}

void GameScene::update(float dt) {
    controller.update(dt);
}

void GameScene::render() {
    window->clear(sf::Color(20, 20, 20));
    controller.render(*window);
    window->display();
}
