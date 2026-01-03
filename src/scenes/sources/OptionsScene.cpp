#include "scenes/headers/OptionsScene.h"
#include "scenes/headers/MenuScene.h"

#include "core/headers/AudioManager.h"
#include "core/headers/SceneManager.h"
#include "scenes/headers/GameScene.h"

OptionsScene::OptionsScene(SceneManager* manager,
                           sf::RenderWindow* window,
                           OptionsReturnTarget target)
    : Scene(manager, window),
      menu(window->getSize().x, window->getSize().y),
      returnTarget(target) {}

void OptionsScene::handleEvent(const sf::Event& event) {

    if (event.type == sf::Event::KeyPressed) {
        menu.handleInput(event.key.code);

        if (event.key.code == sf::Keyboard::Enter &&
            menu.getSelectedIndex() == 3) {

            if (returnTarget == OptionsReturnTarget::Menu)
                manager->resetToMenu(window);
            else if (returnTarget == OptionsReturnTarget::Game)
                manager->popScene();
            }
    }

    menu.handleMouse(event, *window);

    if (menu.buttonBackClicked(event, *window)) {
        if (returnTarget == OptionsReturnTarget::Menu) {
            manager->resetToMenu(window);
        }
        else if (returnTarget == OptionsReturnTarget::Game) {
            manager->popScene();
        }
    }
}

void OptionsScene::update(float) {}

void OptionsScene::render() {
    window->clear(sf::Color::Black);
    menu.draw(*window);
    window->display();
}
