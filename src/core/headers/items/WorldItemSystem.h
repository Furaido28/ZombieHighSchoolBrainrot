#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "WorldItem.h"

// Forward declarations
class TileMap;
class Player;
class GameController;

// Result of trying to pick up an object
enum class PickupResult {
    None,           // No item found or picked
    ItemPicked,     // Standard item picked up
    LuckyBoxPicked,  // Special "Lucky Box" picked up
    KeyFragment
};

// System managing all items on the ground
class WorldItemSystem {
public:
    WorldItemSystem() = default;

    // Spawn (create) items
    void spawnRandom(const TileMap& map, Item item); // Place item at random valid spot
    void spawnAt(const sf::Vector2f& pos, Item item); // Place item at specific X,Y

    // Update logic (interactions)
    void update(Player& player, GameController& game);

    // Draw items to the screen
    void render(sf::RenderWindow& window);

    // Access the list of items
    std::vector<WorldItem>& getItems();

    // Attempt to pick up an item (checks collision with player)
    PickupResult tryPickup(Player& player, int& pickedIndex);

    // Checks if a specific item index is a Lucky Box
    bool isLuckyBox(int index) const;

    // Removes an item from the world (after pickup)
    void removeItem(int index);

    // Remove all items (used when changing levels)
    void clear();

private:
    std::vector<WorldItem> worldItems; // List of active items

    // Helper to find a valid spot on the map (not a wall)
    static sf::Vector2f getRandomFreeTileCenter(const TileMap& map);
};