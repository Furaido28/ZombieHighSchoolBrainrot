#pragma once

#include <SFML/Graphics.hpp>

class SceneManager;

class Scene {
public:
    Scene(SceneManager* manager, sf::RenderWindow* window)
        : manager(manager), window(window) {}

    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;

protected:
    SceneManager* manager;
    sf::RenderWindow* window;
};
