#include "models/headers/Enemy/GenericEnemy.h"
#include "models/headers/Player.h"
#include <cmath>
#include <iostream>

// Constructor: creates an enemy using a shared archetype
GenericEnemy::GenericEnemy(const sf::Vector2f& startPos, const EnemyArchetype& archetype)
    : Enemy(startPos), data(archetype)
{
    // Apply all stats from the archetype to this enemy instance
    applyArchetype(data);
}

// Updates enemy movement each frame
void GenericEnemy::update(float dt, const sf::Vector2f& playerPos) {
    // Direction vector from enemy to player
    sf::Vector2f dir = playerPos - position;

    // Compute the length of the direction vector
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    // Avoid division by zero
    if (length > 0.f) {
        // Normalize the direction vector
        dir /= length;

        // Move the enemy toward the player
        velocity = dir * speed;
        position += velocity * dt;
    }
}