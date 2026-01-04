#pragma once

#include <SFML/Audio.hpp>

#include "../../core/headers/Scene.h"
#include "../../controllers/headers/GameController.h"
#include "../../views/headers/game/InventoryView.h"
#include "views/headers/menu/PauseMenu.h"

// The main gameplay scene where the action happens
class GameScene : public Scene {
public:
    GameScene(SceneManager* manager, sf::RenderWindow* window); // Constructor

    void handleEvent(const sf::Event& event) override; // Handle input (keys, mouse)
    void update(float dt) override; // Update game logic (movement, physics)
    void render() override; // Draw the game to the screen

private:
    GameController controller; // Manages the game rules and entities
    sf::View hudView; // A fixed camera for the UI (HUD)
    sf::Music backgroundMusic; // Music player
    InventoryView inventoryView; // Visual representation of the inventory
    sf::Font font; // Font for text
    sf::Clock levelEndVisualClock; // Timer for level transition
    bool visualTimerStarted = false;

    bool paused; // Is the game currently paused?
    PauseMenu pauseMenu; // Menu when paused
};
