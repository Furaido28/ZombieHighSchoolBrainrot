#pragma once

#include <SFML/Graphics.hpp>

class Player;
class TileMap;

struct AttackInfo;

class PlayerController {
public:
    void update(
        float dt,
        Player& player,
        const TileMap& map,
        AttackInfo& outAttack
    );

private:
    bool isPositionFree(const sf::FloatRect& bbox, const TileMap& map) const;
};
