#pragma once

#include <SFML/Graphics.hpp>
#include "models/headers/Item.h"

struct WorldItem {
    Item item;
    sf::Vector2f position;
    float radius = 40.f;

    sf::FloatRect getBounds() const {
        return {
            position.x - radius,
            position.y - radius,
            radius * 2.f,
            radius * 2.f
        };
    }
};
