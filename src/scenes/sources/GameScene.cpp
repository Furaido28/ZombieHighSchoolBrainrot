#include "../headers/GameScene.h"
#include "../headers/MenuScene.h"
#include "../../core/headers/SceneManager.h"

GameScene::GameScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window), inventoryView(controller.getPlayer().getInventory()) {
}

void GameScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);
    if (event.type == sf::Event::MouseWheelScrolled) {
        Inventory& inv = controller.getPlayer().getInventory();
        int current = inv.getSelectedSlot();

        int maxSlot;
        if (tabPressed)
            maxSlot = 8;
        else
            maxSlot = 2;
        if (event.mouseWheelScroll.delta >0) {
            if (current < maxSlot)
                inv.selectSlot(current +1);
        }
        else if (event.mouseWheelScroll.delta < 0){
            if (current >0)
                inv.selectSlot(current -1);
        }
    }
    if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::Num1)
            controller.getPlayer().getInventory().selectSlot(0);
        if (event.key.code == sf::Keyboard::Num2)
            controller.getPlayer().getInventory().selectSlot(1);
        if (event.key.code == sf::Keyboard::Num3)
            controller.getPlayer().getInventory().selectSlot(2);
        if (event.key.code == sf::Keyboard::Num4)
            controller.getPlayer().getInventory().selectSlot(3);
        if (event.key.code == sf::Keyboard::Num5)
            controller.getPlayer().getInventory().selectSlot(4);
        if (event.key.code == sf::Keyboard::Num6)
            controller.getPlayer().getInventory().selectSlot(5);
        if (event.key.code == sf::Keyboard::Num7)
            controller.getPlayer().getInventory().selectSlot(6);
        if (event.key.code == sf::Keyboard::Num8)
            controller.getPlayer().getInventory().selectSlot(7);
        if (event.key.code == sf::Keyboard::Num9)
            controller.getPlayer().getInventory().selectSlot(8);



        if (event.key.code == sf::Keyboard::Tab)
            tabPressed = true;

        if (event.key.code == sf::Keyboard::Escape)
            manager->changeScene<MenuScene>(window);
    }

    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Tab) {
        tabPressed = false;
        }
    if (event.type == sf::Event::KeyPressed &&
    event.key.code == sf::Keyboard::I) {
        controller.givePotionToPlayer();
    }

}


void GameScene::update(float dt) {
    controller.update(dt);
    inventoryView.update(tabPressed);
}

void GameScene::render() {
    window->clear(sf::Color(20, 20, 20));
    controller.render(*window);
    inventoryView.draw(*window);
    window->display();
}
