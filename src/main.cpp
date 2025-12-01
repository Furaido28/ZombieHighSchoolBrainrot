#include <SFML/Graphics.hpp>
#include "core/SceneManager.h"
#include "scenes/MenuScene.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Zombie High School Brainrot");
    window.setFramerateLimit(60);

    SceneManager manager;
    manager.changeScene<MenuScene>(&window);

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
