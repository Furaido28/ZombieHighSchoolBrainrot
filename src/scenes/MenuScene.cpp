#include "MenuScene.h"
#include "GameScene.h"
#include "core/SceneManager.h"

MenuScene::MenuScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      controller(window->getSize().x, window->getSize().y) {}

void MenuScene::handleEvent(const sf::Event& event) {
    controller.handleEvent(event);

    int c = controller.getSelectedChoice();
    if (c == 0)
        manager->changeScene<GameScene>(window);

    if (c == 1)
        ; // A VENIR : OptionsScene

    if (c == 2)
        window->close();
}

void MenuScene::update(float dt) {
    controller.update(dt);
}

void MenuScene::render() {
    window->clear();
    controller.render(*window);
    window->display();
}
