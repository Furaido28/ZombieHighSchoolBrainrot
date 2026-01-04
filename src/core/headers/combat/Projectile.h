#pragma once

#include <SFML/Graphics.hpp>

// Data structure representing a bullet or spell
struct Projectile {
    sf::Vector2f position; // Current location (x, y)
    sf::Vector2f velocity; // Movement speed and direction

    int damage = 0; // Power of the projectile

    float distanceTraveled = 0.f;   // Tracks how far it has moved
    float maxRange = 0.f;   // Maximum distance before disappearing

    bool active = true; // If false, the projectile is removed from the game

    sf::CircleShape shape; // Visual representation (circle)
};
