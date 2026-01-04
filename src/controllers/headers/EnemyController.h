#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Enemy;
class Player;
class TileMap;

class EnemyController {
public:
    EnemyController() = default;

    void update(
        float dt,
        Player& player,
        const TileMap& map,
        std::vector<std::unique_ptr<Enemy>>& enemies
    );

private:
    bool isPositionFree(
        const sf::FloatRect& bbox,
        const TileMap& map
    ) const;

    bool circlesIntersect(
        const sf::Vector2f& aPos, float aRadius,
        const sf::Vector2f& bPos, float bRadius
    ) const;
};
