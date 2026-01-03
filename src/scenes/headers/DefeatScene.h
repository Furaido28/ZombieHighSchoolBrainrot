#pragma once
#include "core/headers/Scene.h"
#include "views/headers/menu/DefeatMenu.h"

class DefeatScene : public Scene {
public:
    DefeatScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float) override {}
    void render() override;

private:
    DefeatMenu menu;
};
