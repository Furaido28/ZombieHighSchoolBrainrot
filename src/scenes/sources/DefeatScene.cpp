#include "scenes/headers/DefeatScene.h"
#include "scenes/headers/MenuScene.h"
#include "core/headers/SceneManager.h"

DefeatScene::DefeatScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y) {}

void DefeatScene::handleEvent(const sf::Event& event) {
    if (menu.handleEvent(event, *window)) {
        manager->resetToMenu(window);
    }
}

void DefeatScene::render() {
    // Fond sombre
    window->clear(sf::Color(10,10,10));

    sf::RectangleShape overlay;
    overlay.setSize({
        (float)window->getSize().x,
        (float)window->getSize().y
    });
    overlay.setFillColor(sf::Color(0,0,0,200));
    window->draw(overlay);

    menu.draw(*window);
    window->display();
}
