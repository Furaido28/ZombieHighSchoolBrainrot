#include "../headers/WaveManager.h"

#include <iostream>

#include "controllers/headers/GameController.h"
#include "models/headers/Boss/UdinDinDinDun.h"

// ======================================================
// CONSTRUCTOR
// ======================================================
WaveManager::WaveManager(const TileMap& mapRef, GameController& controller)
    : map(mapRef), controller(controller)
{
    // ---------- Scan the map ----------
    // Detect free tiles and boss spawn tiles
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile(x, y);

            // Free tiles used for enemy spawning
            if (tile == '.' || tile == '*'){
                freeTiles.emplace_back(x, y);
            }
            // Boss tiles store position + boss type (1 to 4)
            else if (tile >= '1' && tile <= '4') {
                bossTiles.push_back({
                    {static_cast<int>(x), static_cast<int>(y)},
                    tile - '0'
                });
            }
        }
    }

    // ---------- Wave configuration ----------
    // Each wave defines how many enemies of each type are spawned
    waves.push_back({6, 0, 0, 0});
    waves.push_back({10, 4, 0, 0});
    waves.push_back({14, 8, 3, 0});
    waves.push_back({20, 8, 6, 1});

    // Initialize timers
    preWaveClock.restart();
    firstWaveStarted = false;

    spawnClock.restart();
    waveClock.restart();

    // Debug information
    std::cout << "[DEBUG] freeTiles = " << freeTiles.size() << std::endl;
    std::cout << "[DEBUG] bossTiles = " << bossTiles.size() << std::endl;
}

// ======================================================
// GETTERS
// ======================================================
int WaveManager::getCurrentWave() const {
    // Display wave index starting at 1
    return currentWave + 1;
}

float WaveManager::getTimeLeft() const {
    // No time left if all waves are finished
    if (isFinished()) return 0.f;

    float remaining =
        maxWaveDuration - waveClock.getElapsedTime().asSeconds();

    return std::max(0.f, remaining);
}

// ======================================================
// UPDATE
// ======================================================
void WaveManager::update(float dt, Player& player,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // Stop everything if the player is dead
    if (!player.isAlive())
        return;

    // Handle delay before the first wave
    if (!firstWaveStarted) {
        if (preWaveClock.getElapsedTime().asSeconds() < preWaveDelay) {
            return;
        }
        firstWaveStarted = true;
        waveClock.restart();
    }

    // Stop updating if all waves are completed
    if (currentWave >= static_cast<int>(waves.size()))
        return;

    // Remove dead enemies from the list
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return !e->isAlive();
            }),
        enemies.end()
    );

    const Wave& w = waves[currentWave];
    int totalEnemies = w.simple + w.medium + w.difficult + w.hard;

    // ---------- ENEMY SPAWNING ----------
    if (spawnedInWave < totalEnemies &&
        spawnClock.getElapsedTime().asSeconds() >= spawnDelay)
    {
        spawnEnemy(player, enemies);
        spawnedInWave++;
        spawnClock.restart();
    }

    bool allSpawned = (spawnedInWave >= totalEnemies);
    bool enemiesAlive = !enemies.empty();

    // --------------------------------------------------------------------
    // DEBUG MODE: Skip current wave
    if (debugSkipRequested) {
        std::cout << "[DEBUG] Wave skipped\n";
        enemies.clear();
        spawnedInWave = totalEnemies;
        debugSkipRequested = false;
    }
    // --------------------------------------------------------------------

    // ---------- END OF WAVE ----------
    // Move to next wave when all enemies are spawned and defeated
    if (allSpawned && !enemiesAlive) {
        currentWave++;
        spawnedInWave = 0;
        spawnClock.restart();
        waveClock.restart();
    }
}

// ======================================================
// SPAWN ENEMY
// ======================================================
void WaveManager::spawnEnemy(Player& player,
                             std::vector<std::unique_ptr<Enemy>>& enemies)
{
    const Wave& w = waves[currentWave];
    int index = spawnedInWave;

    // ---------- NORMAL ENEMIES ----------
    if (index < w.simple) {
        auto pos = getSpawnPosition(player);
        const auto& data = enemyFactory.get(EnemyType::Basic);
        enemies.push_back(std::make_unique<GenericEnemy>(pos, data));
    }
    else if (index < w.simple + w.medium) {
        auto pos = getSpawnPosition(player);
        const auto& data = enemyFactory.get(EnemyType::Fast);
        enemies.push_back(std::make_unique<GenericEnemy>(pos, data));
    }
    else if (index < w.simple + w.medium + w.difficult) {
        auto pos = getSpawnPosition(player);
        const auto& data = enemyFactory.get(EnemyType::Tank);
        enemies.push_back(std::make_unique<GenericEnemy>(pos, data));
    }
    // ---------- BOSS ----------
    else if (w.hard > 0) {

        int bossType = 1;
        sf::Vector2f pos = getBossSpawnPosition(bossType);

        std::cout << "[SPAWN] Boss requested | type tile = "
             << bossType
             << " | position = (" << pos.x << ", " << pos.y << ")"
             << std::endl;

        // Spawn the correct boss depending on the tile value
        switch (bossType) {
            case 1: {
                auto boss = std::make_unique<TralaleroTralala>(pos);
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(std::move(boss));
                break;
            }

            case 2: {
                auto boss = std::make_unique<ChimpanziniBananini>(pos);
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(std::move(boss));
                break;
            }

            case 3: {
                auto boss = std::make_unique<UdinDinDinDun>(pos);
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(std::move(boss));
                break;
            }

            case 4: {
                auto boss = std::make_unique<OscarTheCrackhead>(pos);
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(std::move(boss));
                break;
            }
        }
    }
}

// ======================================================
// SPAWN POSITIONS
// ======================================================
sf::Vector2f WaveManager::getSpawnPosition(const Player& player)
{
    // Fallback: spawn on player if no free tile exists
    if (freeTiles.empty())
        return player.getPosition();

    const float tileSize = map.getTileSize();
    const float minDist = 200.f;

    // Try to spawn far enough from the player
    for (int i = 0; i < 20; ++i) {
        const auto& tile = freeTiles[rand() % freeTiles.size()];
        sf::Vector2f pos(
            tile.x * tileSize + tileSize / 2.f,
            tile.y * tileSize + tileSize / 2.f
        );

        sf::Vector2f d = pos - player.getPosition();
        if (d.x * d.x + d.y * d.y >= minDist * minDist)
            return pos;
    }

    // Fallback spawn if distance condition failed
    const auto& tile = freeTiles[rand() % freeTiles.size()];
    return {
        tile.x * tileSize + tileSize / 2.f,
        tile.y * tileSize + tileSize / 2.f
    };
}

sf::Vector2f WaveManager::getBossSpawnPosition(int& outBossType) const
{
    const float tileSize = map.getTileSize();

    // Default boss spawn if no boss tiles exist
    if (bossTiles.empty()) {
        outBossType = 1;
        return {
            map.getWidth() * tileSize / 2.f,
            map.getHeight() * tileSize / 2.f
        };
    }

    // Pick a random boss tile
    const BossTile& bt = bossTiles[rand() % bossTiles.size()];
    outBossType = bt.bossType;

    return {
        bt.pos.x * tileSize + tileSize / 2.f,
        bt.pos.y * tileSize + tileSize / 2.f
    };
}

// Request skipping the current wave (debug)
void WaveManager::requestSkip() {
    debugSkipRequested = true;
}

// Checks if all waves are completed
bool WaveManager::isFinished() const {
    return currentWave >= static_cast<int>(waves.size());
}

// Returns remaining time before the first wave starts
float WaveManager::getTimeBeforeFirstWave() const {
    if (firstWaveStarted) return 0.f;
    return std::max(
        0.f,
        preWaveDelay - preWaveClock.getElapsedTime().asSeconds()
    );
}