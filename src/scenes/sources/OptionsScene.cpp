#include "scenes/headers/OptionsScene.h"
#include "scenes/headers/MenuScene.h"

#include "core/headers/AudioManager.h"
#include "core/headers/SceneManager.h"

OptionsScene::OptionsScene(SceneManager* manager, sf::RenderWindow* window)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y) {}

void OptionsScene::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        menu.handleInput(event.key.code);

        if (event.key.code == sf::Keyboard::Enter &&
            menu.getSelectedIndex() == 3) {
                manager->changeScene<MenuScene>(window);
            }
    }

    menu.handleMouse(event, *window);

    if (menu.buttonBackClicked(event, *window)) {
        manager->changeScene<MenuScene>(window);
    }
}

void OptionsScene::update(float) {}

void OptionsScene::render() {
    window->clear(sf::Color::Black);
    menu.draw(*window);
    window->display();
}
