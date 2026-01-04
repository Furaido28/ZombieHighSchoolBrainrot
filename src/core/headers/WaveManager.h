#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include "models/headers/Enemy.h"
#include "models/headers/Enemy/EnemyFlyweightFactory.h"
#include "models/headers/Enemy/GenericEnemy.h"

#include "models/headers/Boss/TralaleroTralala.h"
#include "models/headers/Boss/ChimpanziniBananini.h"
#include "models/headers/Boss/OscarTheCrackhead.h"

#include "models/headers/Player.h"
#include "models/headers/TileMap.h"

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <memory>


class GameController;
class Enemy;
class Player;
class TileMap;

// Configuration for a single wave: how many enemies of each type
struct Wave {
    int simple;
    int medium;
    int difficult;
    int hard;
};

/* 1 -> ChimpanziniBananini
 * 2 -> TralaleroTralala
 * 3 -> UdinDinDinDun
 * 4 -> OscarTheCrackhead
 */
// Stores a location and ID for a Boss spawn
struct BossTile {
    sf::Vector2i pos;
    int bossType; // 1,2,3,4
};

// Controls the flow of enemy waves, spawning logic, and timers
class WaveManager {
public:
    // Constructor: Needs map data to know where to spawn enemies
    explicit WaveManager(const TileMap& mapRef, GameController& controller);

    // Main update loop: Handles timers and spawns new enemies
    void update(float dt, Player& player,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    // Getters for UI display
    int getCurrentWave() const;
    float getTimeLeft() const;

    // ---------- KEY PASS WAVES ----------
    bool debugSkipRequested = false;
    void requestSkip();

    // Check if all waves are completed
    bool isFinished() const;

    // Time remaining before the first wave starts (initial countdown)
    float getTimeBeforeFirstWave() const;

private:
    // ---------- MAP ----------
    const TileMap& map; // Reference to the level layout
    std::vector<sf::Vector2u> freeTiles; // List of walkable tiles (not walls)
    std::vector<BossTile> bossTiles; // Specific spots for bosses

    // ---------- WAVES ----------
    std::vector<Wave> waves; // List of all wave configurations
    int currentWave = 0; // Current wave index
    int spawnedInWave = 0; // How many enemies spawned in this wave

    float spawnDelay = 1.f; // Time delay between individual spawns
    float maxWaveDuration = 150.f; // Maximum time a wave lasts

    // Logic for the "Get Ready" phase
    sf::Clock preWaveClock;
    const float preWaveDelay = 10.f;
    bool firstWaveStarted = false;

    // Timers
    sf::Clock spawnClock; // Controls enemy spawn rate
    sf::Clock waveClock;  // Controls total wave duration


    // ---------- INTERNAL ----------
    // Creates an enemy and adds it to the list
    void spawnEnemy(Player& player,
                    std::vector<std::unique_ptr<Enemy>>& enemies);

    // Finds a valid spot on the map to spawn an enemy
    sf::Vector2f getSpawnPosition(const Player& player);

    // Finds where a boss should spawn based on the map
    sf::Vector2f getBossSpawnPosition(int &outBossType) const;

    // Factory to create enemies efficiently
    EnemyFlyweightFactory enemyFactory;

    GameController& controller;
};