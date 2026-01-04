#pragma once
#include "core/headers/Scene.h"
#include "views/headers/menu/VictoryMenu.h"

// Scene shown when the player wins the game
class VictoryScene : public Scene {
public:
    VictoryScene(SceneManager* manager, sf::RenderWindow* window); // Constructor

    void handleEvent(const sf::Event& event) override; // Handle inputs
    void update(float) override {} // Update
    void render() override; // Draw the scene

private:
    VictoryMenu menu; // The menu UI
};
