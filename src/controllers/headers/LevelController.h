#pragma once

/* ==========================================================
 * Standard & SFML Includes
 * ========================================================== */
#include <vector>
#include <string>
#include <memory>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

/* ==========================================================
 * Forward Declarations
 * (Avoid heavy includes in header)
 * ========================================================== */
class TileMap;
class Player;
class Enemy;
class WorldItemSystem;
class ItemController;
class WaveManager;

/* ==========================================================
 * LevelController
 * Handles level loading, progression, and transitions
 * ========================================================== */
class LevelController {
public:
    /* =========================
     * Constructor
     * =========================
     * All required systems are passed by reference.
     * WaveManager is passed as a reference to a unique_ptr
     * so it can be reset when loading a new level.
     */
    LevelController(
        TileMap& map,
        Player& player,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        WorldItemSystem& worldItemSystem,
        ItemController& itemController,
        std::unique_ptr<WaveManager>& waveManagerPtr
    );

    /* =========================
     * Update Loop
     * ========================= */
    void update(float dt);

    /* =========================
     * Level Loading & Progression
     * ========================= */

    // Loads a specific level by index
    void loadLevel(int levelIndex);

    // Advances to the next level
    void nextLevel();

    /* =========================
     * Level Ending Logic
     * ========================= */

    // Triggers the end-of-level sequence (key fragment collected)
    void triggerLevelEnd();

    // Returns true if the level is currently ending
    bool isLevelEnding() const;

    // Returns remaining time before level transition
    float getTimeRemaining() const;

    // Spawns the key fragment in the world
    void spawnKeyFragment(const sf::Vector2f& position);

    /* =========================
     * Getters
     * ========================= */
    int getCurrentLevel() const { return currentLevel; }

    /* =========================
     * Level Initialization Helpers
     * ========================= */

    // Places the player at the level spawn point
    void placePlayerAtSpawn();

    // Spawns items present at level start
    void spawnInitialItems();

    // Spawns the lucky box for the level
    void spawnLuckyBox();

private:
    /* ======================================================
     * References to GameController Data
     * ====================================================== */
    TileMap& map;
    Player& player;
    std::vector<std::unique_ptr<Enemy>>& enemies;
    WorldItemSystem& worldItemSystem;
    ItemController& itemController;

    // Reference to the GameController's WaveManager unique_ptr
    std::unique_ptr<WaveManager>& waveManager;

    /* ======================================================
     * Level Data
     * ====================================================== */
    std::vector<std::string> levelMaps;  // Paths or identifiers for level maps
    int currentLevel = 0;

    /* ======================================================
     * End-of-Level State
     * ====================================================== */
    bool levelEnding = false;
    sf::Clock levelEndClock;
    float levelEndDuration = 0.f;

    /* ======================================================
     * Wave Warmup
     * ====================================================== */
    float waveSpawnDelay = 0.f;  // Delay before waves start
};
