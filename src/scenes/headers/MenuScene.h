#pragma once

#include <SFML/Audio.hpp>

#include "../../core/headers/Scene.h"
#include "../../controllers/headers/MenuController.h"

// The main menu screen when starting the game
class MenuScene : public Scene {
public:
    MenuScene(SceneManager* manager, sf::RenderWindow* window); // Constructor

    void handleEvent(const sf::Event& event) override; // Handle inputs
    void update(float dt) override; // Update logic
    void render() override; // Draw the screen

private:
    MenuController controller; // Manages the buttons and user selection
    sf::Music backgroundMusic; // Background audio
};
