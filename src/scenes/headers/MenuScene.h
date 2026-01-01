#pragma once

#include "../../core/headers/Scene.h"
#include "../../controllers/headers/MenuController.h"

class MenuScene : public Scene {
public:
    MenuScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    MenuController controller;
};
