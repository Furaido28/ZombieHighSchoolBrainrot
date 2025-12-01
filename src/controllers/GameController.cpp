#include "GameController.h"

GameController::GameController() {}

void GameController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) player.move({0, -1});
        if (event.key.code == sf::Keyboard::S) player.move({0, 1});
        if (event.key.code == sf::Keyboard::A) player.move({-1, 0});
        if (event.key.code == sf::Keyboard::D) player.move({1, 0});
    }
}

void GameController::update(float dt) {
    player.update(dt);
}

void GameController::render(sf::RenderWindow& window) {
    playerView.render(window, player);
}
