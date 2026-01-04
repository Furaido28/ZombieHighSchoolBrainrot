#pragma once

/* ==========================================================
 * Standard & SFML Includes
 * ========================================================== */
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>

/* ==========================================================
 * Controllers
 * ========================================================== */
#include "CombatController.h"
#include "EnemyController.h"
#include "InputController.h"
#include "ItemController.h"
#include "LuckyBoxController.h"
#include "PlayerController.h"
#include "LevelController.h" // Handles level progression & ending logic

/* ==========================================================
 * Models
 * ========================================================== */
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include "models/headers/Enemy.h"
#include "models/headers/Boss/Boss.h"
#include "models/headers/Item.h"

/* ==========================================================
 * Views
 * ========================================================== */
#include "views/headers/game/PlayerView.h"
#include "views/headers/game/EnemyView.h"
#include "views/headers/game/MapView.h"

/* ==========================================================
 * Core Systems
 * ========================================================== */
#include "core/headers/InputHandler.h"
#include "core/headers/WaveManager.h"
#include "core/headers/combat/AttackSystem.h"
#include "core/headers/combat/ProjectileSystem.h"
#include "core/headers/items/WorldItemSystem.h"

/* ==========================================================
 * GameController
 * Central class coordinating the game loop (MVC style)
 * ========================================================== */
class GameController {
public:
    /* ======================================================
     * Coplien Canonical Form
     * ====================================================== */

    // Default Constructor
    GameController();

    // Destructor
    ~GameController();

    // Constructor of copy
    // delete because std::unique_ptr can't be copied
    GameController(const GameController&) = delete;

    // Assignment Operator
    GameController& operator=(const GameController&) = delete;


    /* =========================
     * Main Game Loop
     * ========================= */
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

    /* =========================
     * Player Accessors
     * ========================= */
    bool isInventoryExpanded() const {
        return player.getInventory().isExpanded();
    }

    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }
    PlayerView& getPlayerView() { return playerView; }

    /* =========================
     * Wave / Combat Information
     * ========================= */
    int getWaveNumber() const {
        return waveManager ? waveManager->getCurrentWave() : 0;
    }

    float getWaveTimeLeft() const {
        return waveManager ? waveManager->getTimeLeft() : 0.f;
    }

    WaveManager* getWaveManager() { return waveManager.get(); }
    const WaveManager* getWaveManager() const { return waveManager.get(); }

    /* =========================
     * Camera
     * ========================= */
    const sf::View& getGameView() const { return gameView; }

    /* =========================
     * Level Logic (delegated to LevelController)
     * ========================= */
    void onKeyFragmentPicked();
    bool isLevelEnding() const;
    float getLevelEndRemainingTime() const;
    void spawnKeyFragmentAt(const sf::Vector2f& position);

    /* =========================
     * Gameplay Helpers
     * ========================= */
    void openLuckyBox(int itemIndex);
    bool isPlayerDead() const;
    bool hasKeyFragment() const;
    bool isOscarDead()const;

private:
    /* ======================================================
     * Controllers (Game Logic)
     * ====================================================== */
    std::unique_ptr<PlayerController> playerController;
    std::unique_ptr<InputController> inputController;
    std::unique_ptr<CombatController> combatController;
    std::unique_ptr<EnemyController> enemyController;
    std::unique_ptr<ItemController> itemController;
    std::unique_ptr<LuckyBoxController> luckyBoxController;
    std::unique_ptr<LevelController> levelController;

    /* ======================================================
     * World Systems
     * ====================================================== */
    WorldItemSystem worldItemSystem;
    std::unique_ptr<WaveManager> waveManager;

    /* ======================================================
     * Player & World State
     * ====================================================== */
    Player player;
    PlayerView playerView;

    TileMap map;
    MapView mapView;

    sf::View gameView;

    std::vector<std::unique_ptr<Enemy>> enemies;
    EnemyView enemyView;

    /* ======================================================
     * Combat Timing
     * ====================================================== */
    float attackCooldown = 0.4f;
    float attackTimer = 0.f;

    /* ======================================================
     * Input / Command System
     * ====================================================== */
    bool tabPressed = false;

    InputHandler inputHandler;
    std::unique_ptr<Command> nextSlotCommand;
    std::unique_ptr<Command> prevSlotCommand;

    /* ======================================================
     * Collision & Utility Methods
     * ====================================================== */

    // Returns the player's bounding size (MVC-compliant)
    sf::Vector2f playerSize() const {
        return player.getSize();
    }

    // Checks if a bounding box is free of collisions
    bool isPositionFree(const sf::FloatRect& boundingBox) const;

    /* ======================================================
     * Level Progression State
     * ====================================================== */
    bool keyFragmentCollected = false;
    bool oscarDead = false;
};