#pragma once

#include <SFML/Graphics.hpp>

// Forward declaration to avoid circular include issues
class SceneManager;

// Abstract base class for all game screens (Menu, Game, Options, etc.)
class Scene {
public:
    // Constructor: Gives the scene access to the Manager and the Window
    Scene(SceneManager* manager, sf::RenderWindow* window)
        : manager(manager), window(window) {}

    // Virtual destructor ensuring derived classes are cleaned up correctly
    virtual ~Scene() = default;

    // Handle inputs like key presses or mouse clicks
    virtual void handleEvent(const sf::Event& event) = 0;

    // Update game logic (movement, timers)
    virtual void update(float dt) = 0;

    // Draw visual elements to the window
    virtual void render() = 0;

protected:
    SceneManager* manager;   // Used to switch scenes
    sf::RenderWindow* window; // Used to draw graphics
};