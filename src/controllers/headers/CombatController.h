#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "models/headers/Player.h"
#include "models/headers/Enemy.h"
#include "models/headers/TileMap.h"
#include "core/headers/combat/ProjectileSystem.h"

class CombatController {
public:
    CombatController();

    void update(
        float dt,
        Player& player,
        const TileMap& map,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        const AttackInfo& attack
    );

    void render(sf::RenderWindow& window);

private:
    ProjectileSystem projectileSystem;

    // Debug
    sf::RectangleShape debugMeleeBox;
    sf::CircleShape debugProjectileRange;
    float debugMeleeTimer = 0.f;
    bool showProjectileRange = false;

    bool circlesIntersect(
        const sf::Vector2f& aPos, float aRadius,
        const sf::Vector2f& bPos, float bRadius
    ) const;
};
