/* ==========================================================
 * Includes
 * ========================================================== */
#include "../headers/LevelController.h"
#include "../../models/headers/TileMap.h"
#include "../../models/headers/Player.h"
#include "../../models/headers/Enemy.h"
#include "../../core/headers/items/WorldItemSystem.h"
#include "../headers/ItemController.h"
#include "../../core/headers/WaveManager.h"
#include "../../core/headers/commands/SelectSlotCommand.h" // Inventory reset helper
#include "../../Constants.h" // TILE_SIZE

#include <iostream>
#include <cmath>
#include <cstdlib> // std::rand

/* ==========================================================
 * Helper Functions
 * ========================================================== */

// Clears all inventory slots and resets selection
// (Logic previously located in GameController)
static void clearInventorySlots(Inventory& inventory) {
    auto& slots = inventory.getSlots();
    for (auto& s : slots) s.reset();
    inventory.selectSlot(0);
}

/* ==========================================================
 * LevelController Constructor
 * ========================================================== */
LevelController::LevelController(
    TileMap& m, 
    Player& p, 
    std::vector<std::unique_ptr<Enemy>>& e, 
    WorldItemSystem& wis, 
    ItemController& ic,
    std::unique_ptr<WaveManager>& wm
) 
: map(m), 
  player(p), 
  enemies(e), 
  worldItemSystem(wis), 
  itemController(ic), 
  waveManager(wm),
  currentLevel(0), 
  levelEnding(false), 
  levelEndDuration(10.f), 
  waveSpawnDelay(0.f)
{
    // List of level map files
    levelMaps = {
        "assets/maps/map1.txt",
        "assets/maps/map2.txt",
        "assets/maps/map3.txt",
        "assets/maps/map4.txt"
    };
}

/* ==========================================================
 * Level Loading
 * ========================================================== */
void LevelController::loadLevel(int levelIndex) {

    // Clamp level index
    if (levelIndex < 0 || levelIndex >= (int)levelMaps.size()) {
        currentLevel = 0;
    } else {
        currentLevel = levelIndex;
    }

    /* =========================
     * Cleanup Previous Level
     * ========================= */

    // Remove all enemies and world items
    enemies.clear();
    worldItemSystem.clear();
    
    /* =========================
     * Load Tile Map
     * ========================= */

    if (!map.loadFromFile(levelMaps[currentLevel], TILE_SIZE)) {
        std::cerr << "ERROR: FAILED TO LOAD TILE MAP " 
                  << levelMaps[currentLevel] << std::endl;
        return;
    }

    /* =========================
     * Reset Wave Manager
     * ========================= */

    // WaveManager must be recreated by GameController later
    waveManager.reset(); 
    
    /* =========================
     * Player Reset
     * ========================= */

    // Place player at spawn point
    placePlayerAtSpawn();

    // Reset inventory ONLY on first level
    if (currentLevel == 0) {
        clearInventorySlots(player.getInventory());
    }
    
    // Full heal
    player.setHealth(100);

    /* =========================
     * Item Spawning
     * ========================= */

    spawnInitialItems();
    spawnLuckyBox();

    /* =========================
     * Level Timers
     * ========================= */

    // Warmup delay before wave starts
    waveSpawnDelay = 10.f;
    levelEnding = false;

    std::cout << "[LEVEL] Loaded level " << currentLevel << std::endl;
}

/* ==========================================================
 * Level Update
 * ========================================================== */
void LevelController::update(float dt) {

    /* =========================
     * Level Ending Transition
     * ========================= */

    if (levelEnding) {
        if (levelEndClock.getElapsedTime().asSeconds() >= levelEndDuration) {
            nextLevel();
        }
    }

    /* =========================
     * Wave Spawn Delay
     * ========================= */

    if (waveSpawnDelay > 0.f) {
        waveSpawnDelay -= dt;
    }
}

/* ==========================================================
 * Level Progression
 * ========================================================== */
void LevelController::nextLevel() {

    // Move to next level
    int nextLvl = currentLevel + 1;
    if (nextLvl >= (int)levelMaps.size()) {
        nextLvl = 0;
    }

    loadLevel(nextLvl);

    // WaveManager must be recreated by GameController after this
}

/* ==========================================================
 * Level End Handling
 * ========================================================== */
void LevelController::triggerLevelEnd() {

    // Prevent multiple triggers
    if (levelEnding) return;

    levelEnding = true;
    levelEndClock.restart();

    // Instantly remove all enemies
    enemies.clear();

    std::cout << "[LEVEL] Level Complete - Enemies cleared. Next in 10s.\n";
}

bool LevelController::isLevelEnding() const {
    return levelEnding;
}

float LevelController::getTimeRemaining() const {
    if (!levelEnding) return 0.f;
    return std::max(
        0.f, 
        levelEndDuration - levelEndClock.getElapsedTime().asSeconds()
    );
}

/* ==========================================================
 * Key Fragment Spawning
 * ========================================================== */
void LevelController::spawnKeyFragment(const sf::Vector2f& position) {

    // No spawning during level ending
    if (levelEnding) return;

    Item fragment;
    fragment.name = "Key Fragment";
    fragment.type = ItemType::KeyFragment;
    fragment.value = 1;
    fragment.sprite.setTexture(itemController.getTexture("key-fragment"));
    fragment.sprite.setPosition(position);
    
    worldItemSystem.spawnAt(position, fragment);

    std::cout << "[LEVEL] Key Fragment spawned.\n";
}

/* ==========================================================
 * Player Spawn Logic
 * ========================================================== */
void LevelController::placePlayerAtSpawn() {

    float tileSizeF = (float)map.getTileSize();
    bool spawnFound = false;

    /* =========================
     * Priority Spawn ('$')
     * ========================= */

    // Look for explicit spawn tile '$'
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile((int)x, (int)y);
            if (tile == '$') {

                float cx = x * tileSizeF + tileSizeF / 2.f;
                float cy = y * tileSizeF + tileSizeF / 2.f;
                player.setPosition(cx, cy);
                spawnFound = true;
                
                std::cout << "[LEVEL] Player spawned at ($): " 
                          << x << ", " << y << std::endl;
                return;
            }
        }
    }

    /* =========================
     * Fallback Spawn ('.')
     * ========================= */

    if (!spawnFound) {
        std::cout << "[LEVEL] No spawn point ($). Using first free tile.\n";
        for (unsigned y = 0; y < map.getHeight(); ++y) {
            for (unsigned x = 0; x < map.getWidth(); ++x) {
                if (map.getTile((int)x, (int)y) == '.') {
                    float cx = x * tileSizeF + tileSizeF / 2.f;
                    float cy = y * tileSizeF + tileSizeF / 2.f;
                    player.setPosition(cx, cy);
                    return;
                }
            }
        }
    }
    
    /* =========================
     * Ultimate Fallback
     * ========================= */

    player.setPosition(tileSizeF + 48.f, tileSizeF + 48.f);
}

/* ==========================================================
 * Initial Item Spawning
 * ========================================================== */
void LevelController::spawnInitialItems() {

    /* =========================
     * Guaranteed Items
     * ========================= */

    // Guaranteed medkit
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemController.getTexture("medkit"));
    worldItemSystem.spawnRandom(map, medkit);

    // Guaranteed starting weapon (Pen)
    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.value = 0;
    pen.damage = 15;
    pen.range = 60.f;
    pen.cooldown = 0.3f;
    pen.isProjectile = false;
    pen.sprite.setTexture(itemController.getTexture("pen"));
    worldItemSystem.spawnRandom(map, pen);
    
    /* =========================
     * Random Extra Items
     * ========================= */

    // 70% chance to spawn additional items
    if (std::rand() % 100 < 70) {

        int extraItems = 3 + std::rand() % 5;

        for (int i = 0; i < extraItems; ++i) {

            Item item;
            int r = std::rand() % 3;
            
            if (r == 0) {
                // Medkit
                item.name = "Medkit";
                item.type = ItemType::Consumable;
                item.value = 60;
                item.sprite.setTexture(itemController.getTexture("medkit"));
            } 
            else if (r == 1) {
                // Weapon depending on level
                if (currentLevel == 0) {
                    item.name = "pen";
                    item.type = ItemType::Weapon;
                    item.damage = 15;
                    item.range = 60.f;
                    item.cooldown = 0.3f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemController.getTexture("pen"));
                }
                else {
                    item.name = "book";
                    item.type = ItemType::Weapon;
                    item.damage = 35;
                    item.range = 80.f;
                    item.cooldown = 0.8f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemController.getTexture("book"));
                }
            } 
            else {
                // Chalk projectile weapon
                item.name = "Chalk";
                item.type = ItemType::Weapon;
                item.damage = 10;
                item.range = 600.f;
                item.cooldown = 0.5f;
                item.isProjectile = true;
                item.projectileSpeed = 500.f;
                item.sprite.setTexture(itemController.getTexture("chalk"));
            }
            
            worldItemSystem.spawnRandom(map, item);
        }
    }
}

/* ==========================================================
 * Lucky Box Spawning
 * ========================================================== */
void LevelController::spawnLuckyBox() {

    // One lucky box per level
    Item luckyBox;
    luckyBox.name = "Lucky Box";
    luckyBox.type = ItemType::LuckyBox;
    luckyBox.isPickable = false;
    luckyBox.sprite.setTexture(itemController.getTexture("luckybox"));
    
    // Spawn randomly on the map
    worldItemSystem.spawnRandom(map, luckyBox);

    std::cout << "[LEVEL] LuckyBox spawned in level " 
              << currentLevel << std::endl;
}