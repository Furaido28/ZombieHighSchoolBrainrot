#include "core/headers/items/WorldItemSystem.h"

#include <cmath>

#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include "controllers/headers/GameController.h"

#include <cstdlib>

// =========================
// RANDOM TILE
// =========================
sf::Vector2f WorldItemSystem::getRandomFreeTileCenter(const TileMap& map) {
    // Collect all free tiles from the map
    std::vector<sf::Vector2i> freeTiles;

    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char t = map.getTile(x, y);

            // Only specific tiles can spawn items
            if (t == '.' || t == '*') {
                freeTiles.emplace_back(x, y);
            }
        }
    }

    // No valid tile found
    if (freeTiles.empty())
        return {0.f, 0.f};

    // Pick a random free tile
    int index = std::rand() % freeTiles.size();
    float tileSize = (float)map.getTileSize();

    // Return the center position of the tile
    return {
        freeTiles[index].x * tileSize + tileSize / 2.f,
        freeTiles[index].y * tileSize + tileSize / 2.f
    };
}

// =========================
// SPAWN
// =========================
void WorldItemSystem::spawnRandom(const TileMap& map, Item item) {
    // Create a new world item at a random free tile
    WorldItem wi;
    wi.item = item;
    wi.position = getRandomFreeTileCenter(map);
    wi.item.sprite.setPosition(wi.position);

    worldItems.push_back(wi);
}

void WorldItemSystem::spawnAt(const sf::Vector2f& pos, Item item) {
    // Create a new world item at a specific position
    WorldItem wi;
    wi.item = item;
    wi.position = pos;
    wi.item.sprite.setPosition(pos);

    worldItems.push_back(wi);
}

// =========================
// UPDATE
// =========================
void WorldItemSystem::update(Player& player, GameController& game) {
    // Iterate backwards to allow safe removal
    for (int i = (int)worldItems.size() - 1; i >= 0; --i) {
        WorldItem& wi = worldItems[i];

        // Distance between player and item
        float dist = std::hypot(
            wi.position.x - player.getPosition().x,
            wi.position.y - player.getPosition().y
        );

        // Player is close enough to interact
        if (dist <= wi.radius) {

            // LuckyBox triggers a special game action
            if (wi.item.type == ItemType::LuckyBox) {
                game.openLuckyBox(i);
                return;
            }

            // Standard pickup logic
            if (wi.item.isPickable) {
                if (player.getInventory().addItem(wi.item)) {
                    worldItems.erase(worldItems.begin() + i);
                }
            }
        }
    }
}

// =========================
// RENDER
// =========================
void WorldItemSystem::render(sf::RenderWindow& window) {
    // Draw all world items
    for (auto& wi : worldItems)
        window.draw(wi.item.sprite);
}

// =========================
// ACCESS
// =========================
std::vector<WorldItem>& WorldItemSystem::getItems() {
    return worldItems;
}

// =========================
// TRY TO PICKUP ITEM
// =========================
PickupResult WorldItemSystem::tryPickup(Player& player, int& pickedIndex) {
    // Iterate backwards to safely remove items
    for (int i = (int)worldItems.size() - 1; i >= 0; --i) {
        WorldItem& wi = worldItems[i];

        float dist = std::hypot(
            wi.position.x - player.getPosition().x,
            wi.position.y - player.getPosition().y
        );

        if (dist <= wi.radius) {

            pickedIndex = i;

            // Special case: lucky box
            if (wi.item.type == ItemType::LuckyBox) {
                return PickupResult::LuckyBoxPicked;
            }

            // Standard item pickup
            if (wi.item.isPickable) {
                if (player.getInventory().addItem(wi.item)) {
                    worldItems.erase(worldItems.begin() + i);
                    return PickupResult::ItemPicked;
                }
            }
        }
    }

    // No item picked
    return PickupResult::None;
}

// =========================
// LUCKYBOX
// =========================
bool WorldItemSystem::isLuckyBox(int index) const {
    if (index < 0 || index >= (int)worldItems.size()) return false;
    return worldItems[index].item.type == ItemType::LuckyBox;
}

void WorldItemSystem::removeItem(int index) {
    // Safe removal check
    if (index < 0 || index >= (int)worldItems.size()) return;
    worldItems.erase(worldItems.begin() + index);
}

// =========================
// CLEAR
// =========================
void WorldItemSystem::clear() {
    // Remove all world items
    worldItems.clear();
}