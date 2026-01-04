#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "WorldItem.h"

class TileMap;
class Player;
class GameController;

class WorldItemSystem {
public:
    WorldItemSystem() = default;

    // Spawn
    void spawnRandom(const TileMap& map, Item item);
    void spawnAt(const sf::Vector2f& pos, Item item);

    // Update / pickup
    void update(Player& player, GameController& game);

    // Render
    void render(sf::RenderWindow& window);

    // Access
    std::vector<WorldItem>& getItems();

    // Essaie de récupérer un item
    void tryPickup(Player& player, GameController& game);

    bool isLuckyBox(int index) const;

    void removeItem(int index);

    // Clear (nouveau niveau)
    void clear();

private:
    std::vector<WorldItem> worldItems;

    static sf::Vector2f getRandomFreeTileCenter(const TileMap& map);
};
