#pragma once

#include <SFML/Audio.hpp>

#include "../../core/headers/Scene.h"
#include "../../views/headers/menu/OptionsMenu.h"

class OptionsScene : public Scene {
public:
    OptionsScene(SceneManager* manager, sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    OptionsMenu menu;
};
