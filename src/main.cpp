#include <SFML/Graphics.hpp>
#include "core/headers/SceneManager.h"
#include "scenes/headers/MenuScene.h"

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Zombie High School Brainrot", sf::Style::None);

    // Astuce : On force la position en haut à gauche (0,0) pour être sûr qu'elle couvre tout
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    SceneManager manager;

    manager.pushScene<MenuScene>(&window);

    sf::Image icon;
    if (icon.loadFromFile("assets/icon.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            manager.getCurrentScene()->handleEvent(event);
        }

        float dt = clock.restart().asSeconds();

        manager.getCurrentScene()->update(dt);
        manager.getCurrentScene()->render();
    }

    return 0;
}
