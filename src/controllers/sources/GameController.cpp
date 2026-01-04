#include "../headers/GameController.h"

/* ==========================================================
 * Standard & Utility Includes
 * ========================================================== */
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

/* ==========================================================
 * Controllers
 * ========================================================== */
#include "controllers/headers/PlayerController.h"
#include "controllers/headers/InputController.h"
#include "controllers/headers/ItemController.h"
#include "controllers/headers/CombatController.h"
#include "controllers/headers/EnemyController.h"
#include "controllers/headers/LuckyBoxController.h"
#include "controllers/headers/LevelController.h"

/* ==========================================================
 * Core Systems
 * ========================================================== */
#include "core/headers/LuckyBoxSystem.h"
#include "core/headers/commands/SelectSlotCommand.h"

/* ==========================================================
 * CONSTRUCTOR (Coplien Canonical Form 1/4)
 * ========================================================== */
GameController::GameController() : player(), playerView(*this) {

    /* =========================
     * Global Initialization
     * ========================= */
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    /* =========================
     * Controllers Initialization
     * ========================= */
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
    enemyController  = std::make_unique<EnemyController>();

    luckyBoxController = std::make_unique<LuckyBoxController>(
        player,
        worldItemSystem,
        *itemController
    );

    /* =========================
     * Level Controller
     * ========================= */
    levelController = std::make_unique<LevelController>(
        map,
        player,
        enemies,
        worldItemSystem,
        *itemController,
        waveManager
    );

    /* =========================
     * Player Setup
     * ========================= */
    player.setSize(48.f, 48.f);

    /* =========================
     * First Level Load
     * ========================= */
    levelController->loadLevel(0);

    /*
     * IMPORTANT:
     * LevelController resets the WaveManager pointer internally.
     * We recreate it here because it depends on GameController.
     */
    waveManager = std::make_unique<WaveManager>(map, *this);
    mapView.load(map);

    // Center the camera on the player
    gameView.setCenter(player.getPosition());
}

// ==========================================================
//DESTRUCTOR (Orthodox Canonical Form 2/4)
// ==========================================================
GameController::~GameController() {
    enemies.clear();

    // Optional log to verify proper cleanup
    std::cout << "[GameController] Resources released and controller destroyed." << std::endl;
}

/* * NOTE ON COPLIEN:
 * The copy methods (Copy Constructor and Assignment Operator)
 * are deleted in the .h file because this class manages unique resources.
 */

/* ==========================================================
 * INPUT HANDLING
 * ========================================================== */
void GameController::handleEvent(const sf::Event& event) {
    inputController->handleEvent(event);
}

/* ==========================================================
 * UPDATE LOOP
 * ========================================================== */
void GameController::update(float dt) {

    /* =========================
     * 1. Input Processing
     * ========================= */
    inputController->update(dt);

    /* =========================
     * 2. Level Logic (timers & transitions)
     * ========================= */
    levelController->update(dt);

    /* =========================
     * 3. Player Update
     * ========================= */
    AttackInfo attack;
    playerController->update(dt, player, map, attack);

    luckyBoxController->update(dt);

    /* =========================
     * 4. Enemies Update
     * ========================= */
    enemyController->update(dt, player, map, enemies);

    /* =========================
     * 5. Combat Resolution
     * ========================= */
    combatController->update(dt, player, map, enemies, attack);

    /* =========================
     * 6. Debug – Skip Wave
     * ========================= */
    static bool kWasPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        if (!kWasPressed && waveManager) {
            waveManager->requestSkip();
            std::cout << "[DEBUG] Skip requested\n";
        }
        kWasPressed = true;
    } else {
        kWasPressed = false;
    }

    /* =========================
     * 7. Wave Management
     * ========================= */
    if (waveManager && !levelController->isLevelEnding()) {
        waveManager->update(dt, player, enemies);
    }

    /* =========================
     * 8. Camera Update
     * ========================= */
    gameView.setCenter(player.getPosition());

    /*
     * NOTE:
     * When a new level is loaded, LevelController resets the WaveManager.
     * We detect this and recreate it here.
     */
    if (!waveManager) {
        waveManager = std::make_unique<WaveManager>(map, *this);
        mapView.load(map);
        gameView.setCenter(player.getPosition());
    }
}

/* ==========================================================
 * RENDERING
 * ========================================================== */
void GameController::render(sf::RenderWindow& window) {

    window.setView(gameView);

    // Map
    window.draw(mapView);

    // World items (dropped loot, key fragments, etc.)
    worldItemSystem.render(window);

    // Enemies
    for (auto& enemy : enemies) {
        enemyView.render(window, *enemy, player.getPosition());
    }

    // Combat effects (projectiles, impacts, etc.)
    combatController->render(window);

    // Player (world layer)
    playerView.renderWorld(window, player);

    // Lucky box UI / screamer (screen-space)
    luckyBoxController->render(window);
}

/* ==========================================================
 * COLLISION & UTILS
 * ========================================================== */
bool GameController::isPositionFree(const sf::FloatRect& bbox) const {

    float tileSize = static_cast<float>(map.getTileSize());

    int x0 = static_cast<int>(std::floor(bbox.left / tileSize));
    int y0 = static_cast<int>(std::floor(bbox.top / tileSize));
    int x1 = static_cast<int>(std::floor((bbox.left + bbox.width  - 0.001f) / tileSize));
    int y1 = static_cast<int>(std::floor((bbox.top  + bbox.height - 0.001f) / tileSize));

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char tile = map.getTile(x, y);
            if (tile == '#' || tile == 'R' || tile == 'B' ||
                tile == 'A' || tile == 'r' || tile == 'T' || tile == '~') {
                return false;
            }
        }
    }
    return true;
}

/* ==========================================================
 * DELEGATION METHODS (Level / Gameplay)
 * ========================================================== */
void GameController::onKeyFragmentPicked() {

    keyFragmentCollected = true;

    Item fragment;
    fragment.name = "Key Fragment";
    fragment.type = ItemType::KeyFragment;
    fragment.sprite.setTexture(itemController->getTexture("key-fragment"));

    player.getInventory().addKeyFragment(fragment);
    levelController->triggerLevelEnd();
}

bool GameController::isLevelEnding() const {
    return levelController->isLevelEnding();
}

float GameController::getLevelEndRemainingTime() const {
    return levelController->getTimeRemaining();
}

void GameController::spawnKeyFragmentAt(const sf::Vector2f& pos) {
    if (levelController->getCurrentLevel() < 3)
        levelController->spawnKeyFragment(pos);
    else {
        oscarDead = true;
    }
}

void GameController::openLuckyBox(int itemIndex) {
    luckyBoxController->openLuckyBox(itemIndex);
}

bool GameController::isPlayerDead() const {
    return !player.isAlive();
}

bool GameController::hasKeyFragment() const {
    return keyFragmentCollected;
}
bool GameController::isOscarDead() const {
    return oscarDead;
}