#pragma once

#include <SFML/Audio.hpp>
#include "../../core/headers/Scene.h"
#include "../../views/headers/menu/OptionsMenu.h"

enum class OptionsReturnTarget {
    Menu,
    Game
};

class OptionsScene : public Scene {
public:
    OptionsScene(SceneManager* manager,
                 sf::RenderWindow* window,
                 OptionsReturnTarget target);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    OptionsMenu menu;
    OptionsReturnTarget returnTarget;
};
