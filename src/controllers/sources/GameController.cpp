#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Pour std::remove_if

#include "controllers/headers/PlayerController.h"
#include "core/headers/LuckyBoxSystem.h"
#include "core/headers/commands/SelectSlotCommand.h"

// =========================
// CONSTRUCTOR
// =========================
GameController::GameController() : player(), playerView(*this) {
    // =========================
    // INIT GLOBALE (UNE FOIS)
    // =========================
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    // Inventory& inv = player.getInventory();

    // =========================
    // INIT CONTROLLERS
    // =========================
    playerController = std::make_unique<PlayerController>();

    inputController = std::make_unique<InputController>(
        player,
        player.getInventory(),
        worldItemSystem,
        *this
    );

    itemController = std::make_unique<ItemController>(
        worldItemSystem,
        *this
    );

    combatController = std::make_unique<CombatController>();

    enemyController = std::make_unique<EnemyController>();

    luckyBoxController = std::make_unique<LuckyBoxController>(
        player,
        worldItemSystem,
        *itemController
    );



    player.setSize(48.f, 48.f);

    // =========================
    // PREMIER NIVEAU
    // =========================
    currentLevel = 0;
    initLevel(currentLevel);
}

// =========================ds
// INPUT EVENTS
// =========================
void GameController::handleEvent(const sf::Event& event) {
    inputController->handleEvent(event);
}

// =========================
// UPDATE
// =========================
void GameController::update(float dt) {
    // 1. INPUT (OBLIGATOIRE)
    inputController->update(dt);

    // 2. PLAYER (movement + attack decision)
    AttackInfo attack;
    playerController->update(dt, player, map, attack);

    luckyBoxController->update(dt);

    // 3. ENEMIES
    enemyController->update(dt, player, map, enemies);

    // 4. COMBAT
    combatController->update(dt, player, map, enemies, attack);

    // =========================
    // 7. DEBUG – SKIP WAVE
    // =========================

    static bool kWasPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        if (!kWasPressed && waveManager) {
            waveManager->requestSkip();
            std::cout << "[DEBUG] Skip requested\n";
        }
        kWasPressed = true;
    }
    else {
        kWasPressed = false;
    }

    // =========================
    // 8. WAVES
    // =========================
    if (waveManager && !levelEnding)
        waveManager->update(dt, player, enemies);

    // =========================
    // 9. CAMERA
    // =========================
    gameView.setCenter(player.getPosition());
    if (levelEnding) {
        if (levelEndClock.getElapsedTime().asSeconds() >= levelEndDuration) {
            goToNextLevel();
        }
    }
}

// =========================
// RENDER
// =========================
void GameController::render(sf::RenderWindow& window) {
    window.setView(gameView);

    window.draw(mapView);

    worldItemSystem.render(window);

    for (auto& enemy : enemies)
        enemyView.render(window, *enemy, player.getPosition());

    combatController->render(window);

    playerView.renderWorld(window, player);

    // =========================
    // SCREAMER RENDER (CENTERED)
    // =========================
    luckyBoxController->render(window);
}

// =========================
// UTILS
// =========================
bool GameController::isPositionFree(const sf::FloatRect& bbox) const {
    float tileSizeF = (float)map.getTileSize();

    int x0 = (int)std::floor(bbox.left / tileSizeF);
    int y0 = (int)std::floor(bbox.top / tileSizeF);
    int x1 = (int)std::floor((bbox.left + bbox.width - 0.001f) / tileSizeF);
    int y1 = (int)std::floor((bbox.top + bbox.height - 0.001f) / tileSizeF);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char t = map.getTile(x, y);
            if (t == '#' || t=='R' || t=='B' || t=='A' || t=='r' || t=='T' || t=='~') return false;
        }
    }
    return true;
}

void GameController::placePlayerAtFirstFreeTile() {
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

                // Optionnel : On peut remplacer le '$' par un sol normal '.' une fois le spawn trouvé
                // pour ne pas qu'il reste un symbole bizarre si on repasse dessus.
                // map.setTile(x, y, '.');

                std::cout << "[GAME] Player spawned at specific point ($): " << x << ", " << y << std::endl;
                return; // On arrête de chercher, on a trouvé.
            }
        }
    }

    // 2. Si aucun '$' n'est trouvé, on utilise l'ancienne méthode (premier sol libre '.')
    if (!spawnFound) {
        std::cout << "[GAME] No spawn point ($) found. Searching for first free tile..." << std::endl;
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

    // 3. Sécurité ultime si la map est pleine de murs (ne devrait pas arriver)
    player.setPosition(tileSizeF + playerSize().x, tileSizeF + playerSize().y);
}

void GameController::onKeyFragmentPicked() {
    if (levelEnding) {
        return;
    }
    levelEnding = true;
    levelEndClock.restart();

    enemies.clear();      // Supprime tous les zombies instantanément
    std::cout << "[GAME] Level Complete - Enemies cleared.\n";
}
bool GameController::isLevelEnding() const {
    return levelEnding;
}
float GameController::getLevelEndRemainingTime() const {
    if (!levelEnding)
        return 0.f;
    float elapsed = levelEndClock.getElapsedTime().asSeconds();
    return std::max(0.f, levelEndDuration - elapsed);
}

static void clearInventorySlots(Inventory& inventory) {
    auto& slots = inventory.getSlots();
    for (auto& s : slots) s.reset();
    inventory.selectSlot(0);
}
void GameController::initLevel(int levelIndex) {
    // clean the game
    enemies.clear();
    worldItemSystem.clear();

    //load map
    if (!map.loadFromFile(levelMaps[levelIndex], TILE_SIZE)) {
        std::cerr << "ERROR: FAILED TO LOAD TILE MAP" << levelMaps[levelIndex]<< std::endl;
        return;
    }

    //Rebuild wave manager + map view
    waveManager = std::make_unique<WaveManager>(map, *this);
    mapView.load(map);

    //Reset Player position + inventory (except key)
    placePlayerAtFirstFreeTile();

    if (levelIndex == 0) {
        clearInventorySlots(player.getInventory());
    }

    player.setHealth(100);

    // --- 5) Respawn items ---
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemController->getTexture("medkit"));
    worldItemSystem.spawnRandom(map, medkit);

    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.value = 0;
    pen.damage = 15;
    pen.range = 60.f;
    pen.cooldown = 0.3f;
    pen.isProjectile = false;
    pen.sprite.setTexture(itemController->getTexture("pen"));
    worldItemSystem.spawnRandom(map, pen);


    if (std::rand() % 100 < 70) {
        int extraItems = 3 + std::rand() % 5;
        for (int i = 0; i < extraItems; ++i) {
            Item item;
            int r = std::rand() % 3;

            if (r == 0) {
                item.name = "Medkit";
                item.type = ItemType::Consumable;
                item.value = 60;
                item.sprite.setTexture(itemController->getTexture("medkit"));
            } else if (r == 1) {
                if (currentLevel == 0) {
                    item.name = "pen";
                    item.type = ItemType::Weapon;
                    item.damage = 15;
                    item.range = 60.f;
                    item.cooldown = 0.3f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemController->getTexture("pen"));
                }
                else {
                    item.name = "book";
                    item.type = ItemType::Weapon;
                    item.damage = 35;
                    item.range = 80.f;
                    item.cooldown = 0.8f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemController->getTexture("book"));
                }
            } else {
                item.name = "Chalk";
                item.type = ItemType::Weapon;
                item.damage = 10;
                item.range = 600.f;
                item.cooldown = 0.5f;
                item.isProjectile = true;
                item.projectileSpeed = 500.f;
                item.sprite.setTexture(itemController->getTexture("chalk"));
            }

            worldItemSystem.spawnRandom(map, item);
        }
    }

    //================
    //LUCKY BOX SPAWN
    //================

    // 1. ONE LUCKY BOX PER LEVEL
    Item luckyBox;
    luckyBox.name = "Lucky Box";
    luckyBox.type = ItemType::LuckyBox;
    luckyBox.isPickable = false;

    // 2. SPRITE
    luckyBox.sprite.setTexture(itemController->getTexture("luckybox"));

    // SPAWN THE BOX
    worldItemSystem.spawnRandom(map, luckyBox);

    std::cout << "[GAME] LuckyBox spawned in level " << currentLevel << std::endl;

    // --- 6) Reset camera ---
    gameView.setCenter(player.getPosition());
}
void GameController::goToNextLevel() {
    levelEnding = false;
    currentLevel++;
    if (currentLevel >= (int)levelMaps.size()) {
        currentLevel = 0;
    }
    initLevel(currentLevel);
}

void GameController::spawnKeyFragmentAt(const sf::Vector2f &pos) {
    if (levelEnding)return;
    Item fragment;
    fragment.name = "Key Fragment";
    fragment.type = ItemType::KeyFragment;
    fragment.value = 1;

    //sprite & position
    fragment.sprite.setTexture(itemController->getTexture("key-fragment"));
    fragment.sprite.setPosition(pos);

    worldItemSystem.spawnAt(pos, fragment);

    std::cout << "[GAME] Key Fragment spawned at boss position\n";
}

void GameController::openLuckyBox(int itemIndex) {
    luckyBoxController->openLuckyBox(itemIndex);
}

bool GameController::isPlayerDead() const {
    return !player.isAlive();
}