#pragma once
#include "core/headers/Scene.h"
#include "views/headers/menu/DefeatMenu.h"

// Scene shown when the player loses
class DefeatScene : public Scene {
public:
    DefeatScene(SceneManager* manager, sf::RenderWindow* window); // Constructor

    void handleEvent(const sf::Event& event) override; // Handle user inputs (mouse/keyboard)
    void update(float) override {} // Update
    void render() override; // Draw everything to the screen

private:
    DefeatMenu menu; // The menu with buttons
};