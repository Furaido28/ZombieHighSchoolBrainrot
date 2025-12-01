#pragma once
#include <SFML/Graphics.hpp>
#include "../../models/Player.h"

class PlayerView {
public:
    PlayerView();
    void render(sf::RenderWindow& window, const Player& player);

private:
    sf::CircleShape m_shape;
};