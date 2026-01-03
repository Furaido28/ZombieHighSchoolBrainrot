#include "scenes/headers/VictoryScene.h"
#include "scenes/headers/MenuScene.h"
#include "core/headers/SceneManager.h"

VictoryScene::VictoryScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y) {}

void VictoryScene::handleEvent(const sf::Event& event) {
    if (menu.handleEvent(event, *window)) {
        manager->resetToMenu(window);
    }
}

void VictoryScene::render() {
    sf::RectangleShape overlay;
    overlay.setSize({
        static_cast<float>(window->getSize().x),
        static_cast<float>(window->getSize().y)
    });
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    window->draw(overlay);

    menu.draw(*window);

    window->display();
}
