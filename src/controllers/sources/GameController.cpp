#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Pour std::remove_if

#include "controllers/headers/PlayerController.h"
#include "controllers/headers/InputController.h"
#include "controllers/headers/ItemController.h"
#include "controllers/headers/CombatController.h"
#include "controllers/headers/EnemyController.h"
#include "controllers/headers/LuckyBoxController.h"
#include "controllers/headers/LevelController.h" // <--- NEW

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

    // NOUVEAU : Initialisation LevelController
    levelController = std::make_unique<LevelController>(
        map,
        player,
        enemies,
        worldItemSystem,
        *itemController,
        waveManager
    );

    player.setSize(48.f, 48.f);

    // =========================
    // PREMIER NIVEAU
    // =========================
    // On charge le niveau 0 via le LevelController
    levelController->loadLevel(0);

    // IMPORTANT: Comme LevelController a reset le pointeur waveManager, on doit le recréer
    // ici car il dépend de *this (GameController)
    waveManager = std::make_unique<WaveManager>(map, *this);
    mapView.load(map);

    // Reset camera center
    gameView.setCenter(player.getPosition());
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

    // NOUVEAU: LEVEL UPDATE (timers)
    levelController->update(dt);

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
    // Check level ending via LevelController
    if (waveManager && !levelController->isLevelEnding())
        waveManager->update(dt, player, enemies);

    // =========================
    // 9. CAMERA
    // =========================
    gameView.setCenter(player.getPosition());

    // Note: La logique de transition "levelEnding" -> goToNextLevel est maintenant interne au LevelController.
    // Cependant, si le niveau a changé dans LevelController, nous devons re-initialiser le WaveManager.
    // Pour l'instant, c'est un point délicat sans "signal".
    // SOLUTION SIMPLE : On recrée le WM à chaque frame si on détecte qu'il est null (car reset par LevelController::loadLevel)
    if (!waveManager) {
        waveManager = std::make_unique<WaveManager>(map, *this);
        mapView.load(map);
        // On recentre aussi la caméra au cas où
        gameView.setCenter(player.getPosition());
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

// DELEGATION METHODS

void GameController::onKeyFragmentPicked() {
    levelController->triggerLevelEnd();
}
bool GameController::isLevelEnding() const {
    return levelController->isLevelEnding();
}
float GameController::getLevelEndRemainingTime() const {
    return levelController->getTimeRemaining();
}

void GameController::spawnKeyFragmentAt(const sf::Vector2f &pos) {
    levelController->spawnKeyFragment(pos);
}

void GameController::openLuckyBox(int itemIndex) {
    luckyBoxController->openLuckyBox(itemIndex);
}

bool GameController::isPlayerDead() const {
    return !player.isAlive();
}