#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Projectile.h"

// Forward declarations to avoid circular dependencies
class Enemy;
class TileMap;

// Manages all active projectiles (chalk)
class ProjectileSystem {
public:
    ProjectileSystem() = default;

    // Create and launch a new projectile
    void spawn(const sf::Vector2f& startPos,
               const sf::Vector2f& velocity,
               int damage,
               float maxRange);

    // Update positions and check for collisions (Walls & Enemies)
    void update(float dt,
                const TileMap& map,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    // Draw all active projectiles
    void render(sf::RenderWindow& window);

    // Remove all projectiles (used when resetting the game)
    void clear();

private:
    std::vector<Projectile> projectiles; // List of active projectiles

    // Helper to check if a projectile hit a wall
    bool isPositionFree(const sf::FloatRect& bbox,
                        const TileMap& map) const;
};