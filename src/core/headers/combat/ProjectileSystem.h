#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Projectile.h"

class Enemy;
class TileMap;

class ProjectileSystem {
public:
    ProjectileSystem() = default;

    // Spawn un projectile
    void spawn(const sf::Vector2f& startPos,
               const sf::Vector2f& velocity,
               int damage,
               float maxRange);

    // Update logique + collisions
    void update(float dt,
                const TileMap& map,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    // Rendu
    void render(sf::RenderWindow& window);

    // Reset (changement de niveau)
    void clear();

private:
    std::vector<Projectile> projectiles;

    bool isPositionFree(const sf::FloatRect& bbox,
                        const TileMap& map) const;
};
