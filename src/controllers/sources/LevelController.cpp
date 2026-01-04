#include "../headers/LevelController.h"
#include "../../models/headers/TileMap.h"
#include "../../models/headers/Player.h"
#include "../../models/headers/Enemy.h"
#include "../../core/headers/items/WorldItemSystem.h"
#include "../headers/ItemController.h"
#include "../../core/headers/WaveManager.h"
#include "../../core/headers/commands/SelectSlotCommand.h" // Pour le reset inventaire (si besoin de logique spécifique)
#include "../../Constants.h" // TILE_SIZE

#include <iostream>
#include <cmath>
#include <cstdlib> // Pour std::rand

// Helper local pour reset l'inventaire (comme c'était dans GameController)
static void clearInventorySlots(Inventory& inventory) {
    auto& slots = inventory.getSlots();
    for (auto& s : slots) s.reset();
    inventory.selectSlot(0);
}

LevelController::LevelController(
    TileMap& m, 
    Player& p, 
    std::vector<std::unique_ptr<Enemy>>& e, 
    WorldItemSystem& wis, 
    ItemController& ic,
    std::unique_ptr<WaveManager>& wm
) 
: map(m), player(p), enemies(e), worldItemSystem(wis), itemController(ic), waveManager(wm),
  currentLevel(0), levelEnding(false), levelEndDuration(10.f), waveSpawnDelay(0.f)
{
    levelMaps = {
        "assets/maps/map1.txt",
        "assets/maps/map2.txt",
        "assets/maps/map3.txt",
        "assets/maps/map4.txt"
    };
}

void LevelController::loadLevel(int levelIndex) {
    if (levelIndex < 0 || levelIndex >= (int)levelMaps.size()) {
        currentLevel = 0;
    } else {
        currentLevel = levelIndex;
    }

    // 1. clean the game
    enemies.clear();
    worldItemSystem.clear();
    
    // 2. load map
    if (!map.loadFromFile(levelMaps[currentLevel], TILE_SIZE)) { // TILE_SIZE hardcodé à 32 ici ou include constant
        std::cerr << "ERROR: FAILED TO LOAD TILE MAP " << levelMaps[currentLevel] << std::endl;
        return;
    }

    // 3. Reset WaveManager (On doit le recréer pour la nouvelle map)
    // Note: On laisse le GameController gérer la création finale du WaveManager car il a besoin de *this (GameController)
    // Mais on peut le reset ici pour être propre.
    waveManager.reset(); 
    
    // 4. Reset Player position ($ logic)
    placePlayerAtSpawn();

    // 5. Reset Inventaire (except key) - UNIQUEMENT au niveau 0
    if (currentLevel == 0) {
        clearInventorySlots(player.getInventory());
    }
    
    // Soin complet
    player.setHealth(100);

    // 6. Spawn Items initial
    spawnInitialItems();
    spawnLuckyBox();

    // 7. Timer de début (Warmup)
    waveSpawnDelay = 10.f;
    levelEnding = false;

    std::cout << "[LEVEL] Loaded level " << currentLevel << std::endl;
}

void LevelController::update(float dt) {
    // Gestion fin de niveau (Level Ending transition)
    if (levelEnding) {
        if (levelEndClock.getElapsedTime().asSeconds() >= levelEndDuration) {
            nextLevel();
        }
    }

    // Gestion délai début de vague (Warmup timer)
    if (waveSpawnDelay > 0.f) {
        waveSpawnDelay -= dt;
    }
}

void LevelController::nextLevel() {
    // Passage au niveau suivant
    int nextLvl = currentLevel + 1;
    if (nextLvl >= (int)levelMaps.size()) {
        nextLvl = 0;
    }
    loadLevel(nextLvl);
    // Important : Il faudra recréer le WaveManager côté GameController après cet appel
}

void LevelController::triggerLevelEnd() {
    if (levelEnding) return;
    levelEnding = true;
    levelEndClock.restart();
    enemies.clear(); // Supprime tous les zombies instantanément
    std::cout << "[LEVEL] Level Complete - Enemies cleared. Next in 10s.\n";
}

bool LevelController::isLevelEnding() const {
    return levelEnding;
}

float LevelController::getTimeRemaining() const {
    if (!levelEnding) return 0.f;
    return std::max(0.f, levelEndDuration - levelEndClock.getElapsedTime().asSeconds());
}

void LevelController::spawnKeyFragment(const sf::Vector2f& position) {
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

void LevelController::placePlayerAtSpawn() {
    float tileSizeF = (float)map.getTileSize();
    bool spawnFound = false;

    // 1. On cherche d'abord le symbole '$' (Spawn Point)
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile((int)x, (int)y);
            if (tile == '$') {
                // On a trouvé le point de spawn !
                float cx = x * tileSizeF + tileSizeF / 2.f;
                float cy = y * tileSizeF + tileSizeF / 2.f;
                player.setPosition(cx, cy);
                spawnFound = true;
                
                std::cout << "[LEVEL] Player spawned at specific point ($): " << x << ", " << y << std::endl;
                return; // On arrête de chercher, on a trouvé.
            }
        }
    }

    // 2. Si aucun '$' n'est trouvé, on utilise l'ancienne méthode (premier sol libre '.')
    if (!spawnFound) {
        std::cout << "[LEVEL] No spawn point ($) found. Searching for first free tile..." << std::endl;
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
    
    // 3. Sécurité ultime
    player.setPosition(tileSizeF + 48.f, tileSizeF + 48.f);
}

void LevelController::spawnInitialItems() {
    // --- 5) Respawn items ---
    
    // Medkit garanti
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemController.getTexture("medkit"));
    worldItemSystem.spawnRandom(map, medkit);

    // Arme de départ (Pen) garanti
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
    
    // Items aléatoires (70% de chance)
    if (std::rand() % 100 < 70) {
        int extraItems = 3 + std::rand() % 5;
        for (int i = 0; i < extraItems; ++i) {
            Item item;
            int r = std::rand() % 3;
            
            if (r == 0) {
                item.name = "Medkit";
                item.type = ItemType::Consumable;
                item.value = 60;
                item.sprite.setTexture(itemController.getTexture("medkit"));
            } else if (r == 1) {
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
            } else {
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

void LevelController::spawnLuckyBox() {
    //================
    //LUCKY BOX SPAWN
    //================

    // 1. ONE LUCKY BOX PER LEVEL
    Item luckyBox;
    luckyBox.name = "Lucky Box";
    luckyBox.type = ItemType::LuckyBox;
    luckyBox.isPickable = false;
    luckyBox.sprite.setTexture(itemController.getTexture("luckybox"));
    
    // SPAWN THE BOX
    worldItemSystem.spawnRandom(map, luckyBox);
    std::cout << "[LEVEL] LuckyBox spawned in level " << currentLevel << std::endl;
}