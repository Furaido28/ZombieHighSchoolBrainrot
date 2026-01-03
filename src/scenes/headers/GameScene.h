#pragma once

#include <SFML/Audio.hpp>

#include "../../core/headers/Scene.h"
#include "../../controllers/headers/GameController.h"
#include "../../views/headers/game/InventoryView.h"
#include "views/headers/menu/PauseMenu.h"

class GameScene : public Scene {
public:
    GameScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    GameController controller;
    sf::View hudView;
    sf::Music backgroundMusic;
    InventoryView inventoryView;
    sf::Font font;
    sf::Clock levelEndVisualClock;
    bool visualTimerStarted = false;

    bool paused;
    PauseMenu pauseMenu;
};
