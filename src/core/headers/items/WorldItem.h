#pragma once

#include <SFML/Graphics.hpp>
#include "models/headers/Item.h"

// Represents an item on the ground in the world
struct WorldItem {
    Item item;              // The actual item data (stats, type)
    sf::Vector2f position;  // Location in the world (x, y)
    float radius = 40.f;    // Size of the pickup area

    // Helper to get the hitbox
    sf::FloatRect getBounds() const {
        return {
            position.x - radius,
            position.y - radius,
            radius * 2.f,
            radius * 2.f
        };
    }
};