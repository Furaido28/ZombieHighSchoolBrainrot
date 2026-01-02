#include "../headers/GameScene.h"
#include "../headers/MenuScene.h"
#include "../../core/headers/SceneManager.h"

GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window), inventoryView(controller.getPlayer().getInventory()) {
}

void GameScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);
}


void GameScene::update(float dt) {
    controller.update(dt);
    inventoryView.update(controller.isInventoryExpanded());
}

void GameScene::render() {
    window->clear(sf::Color(20, 20, 20));
    controller.render(*window);
    inventoryView.draw(*window);
    window->display();
}
