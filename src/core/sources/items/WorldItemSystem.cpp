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
    std::vector<sf::Vector2i> freeTiles;

    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char t = map.getTile(x, y);
            if (t == '.' || t == '*') {
                freeTiles.emplace_back(x, y);
            }
        }
    }

    if (freeTiles.empty())
        return {0.f, 0.f};

    int index = std::rand() % freeTiles.size();
    float tileSize = (float)map.getTileSize();

    return {
        freeTiles[index].x * tileSize + tileSize / 2.f,
        freeTiles[index].y * tileSize + tileSize / 2.f
    };
}

// =========================
// SPAWN
// =========================
void WorldItemSystem::spawnRandom(const TileMap& map, Item item) {
    WorldItem wi;
    wi.item = item;
    wi.position = getRandomFreeTileCenter(map);
    wi.item.sprite.setPosition(wi.position);

    worldItems.push_back(wi);
}

void WorldItemSystem::spawnAt(const sf::Vector2f& pos, Item item) {
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
    for (int i = (int)worldItems.size() - 1; i >= 0; --i) {
        WorldItem& wi = worldItems[i];

        float dist = std::hypot(
            wi.position.x - player.getPosition().x,
            wi.position.y - player.getPosition().y
        );

        if (dist <= wi.radius) {
            if (wi.item.type == ItemType::LuckyBox) {
                game.openLuckyBox(i);
                return;
            }
        }
    }
}

// =========================
// RENDER
// =========================
void WorldItemSystem::render(sf::RenderWindow& window) {
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
    for (int i = (int)worldItems.size() - 1; i >= 0; --i) {
        WorldItem& wi = worldItems[i];

        float dist = std::hypot(
            wi.position.x - player.getPosition().x,
            wi.position.y - player.getPosition().y
        );

        if (dist <= wi.radius) {

            pickedIndex = i;

            if (wi.item.type == ItemType::LuckyBox) {
                return PickupResult::LuckyBoxPicked;
            }

            if (wi.item.type == ItemType::KeyFragment) {
                worldItems.erase(worldItems.begin()+i);
                return PickupResult::KeyFragment;
            }
            if (wi.item.isPickable) {
                if (player.getInventory().addItem(wi.item)) {
                    worldItems.erase(worldItems.begin() + i);
                    return PickupResult::ItemPicked;
                }
            }
        }
    }

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
    if (index < 0 || index >= (int)worldItems.size()) return;
    worldItems.erase(worldItems.begin() + index);
}

// =========================
// CLEAR
// =========================
void WorldItemSystem::clear() {
    worldItems.clear();
}
