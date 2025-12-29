#pragma once
#include <SFML/Graphics.hpp>
#include "../../views/headers/menu/MenuView.h"

class MenuController {
public:
    MenuController(float width, float height);

    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

    int getSelectedChoice() const;

private:
    Menu menu;
    int choice;
};
