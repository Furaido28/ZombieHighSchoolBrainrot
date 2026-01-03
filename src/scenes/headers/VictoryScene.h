#pragma once
#include "core/headers/Scene.h"
#include "views/headers/menu/VictoryMenu.h"

class VictoryScene : public Scene {
public:
    VictoryScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float) override {}
    void render() override;

private:
    VictoryMenu menu;
};
