#pragma once

#include <SFML/Graphics.hpp>

struct Projectile {
    sf::Vector2f position;
    sf::Vector2f velocity;

    int damage = 0;

    float distanceTraveled = 0.f;
    float maxRange = 0.f;

    bool active = true;

    sf::CircleShape shape;
};
