#pragma once

#include <SFML/Audio.hpp>
#include "../../core/headers/Scene.h"
#include "../../views/headers/menu/OptionsMenu.h"

// Enum to remember where we came from (Main Menu or during Game)
enum class OptionsReturnTarget {
    Menu,
    Game
};

// Scene for changing settings (Sound, Difficulty, etc.)
class OptionsScene : public Scene {
public:
    OptionsScene(SceneManager* manager,
                 sf::RenderWindow* window,
                 OptionsReturnTarget target); // Constructor with target

    void handleEvent(const sf::Event& event) override; // Handle input
    void update(float dt) override; // Update logic
    void render() override; // Draw the scene

private:
    OptionsMenu menu; // The UI for options
    OptionsReturnTarget returnTarget; // Where to go back when closing options
};
