#pragma once

#include "../../core/headers/Scene.h"
#include "../../controllers/headers/GameController.h"
#include "../../views/headers/game/InventoryView.h"

class GameScene : public Scene {
public:
    GameScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    GameController controller;
    InventoryView inventoryView;
    bool tabPressed = false;
};
