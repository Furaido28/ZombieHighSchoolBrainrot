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
    // ---------- Scan map ----------
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile(x, y);

            if (tile == '.' || tile == '*'){
                freeTiles.emplace_back(x, y);
            }
            else if (tile >= '1' && tile <= '4') {
                bossTiles.push_back({
                    {static_cast<int>(x), static_cast<int>(y)},
                    tile - '0'
                });
            }
        }
    }

    // ---------- 4 waves ----------
    waves.push_back({6, 0, 0, 1});
    // waves.push_back({6, 0, 0, 0});
    // waves.push_back({10, 4, 0, 0});
    // waves.push_back({14, 8, 3, 0});
    // waves.push_back({20, 8, 6, 1});

    preWaveClock.restart();
    firstWaveStarted = false;

    spawnClock.restart();
    waveClock.restart();

    std::cout << "[DEBUG] freeTiles = " << freeTiles.size() << std::endl;
    std::cout << "[DEBUG] bossTiles = " << bossTiles.size() << std::endl;

}

// ======================================================
// GETTERS
// ======================================================
int WaveManager::getCurrentWave() const {
    return currentWave + 1;
}

    float WaveManager::getTimeLeft() const {
    if (isFinished())return 0.f;
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
    if (!player.isAlive())
        return;

    if (!firstWaveStarted) {
        if (preWaveClock.getElapsedTime().asSeconds() < preWaveDelay) {
            return; // on ne lance rien encore
        }
        firstWaveStarted = true;
        waveClock.restart();
    }

    if (currentWave >= static_cast<int>(waves.size()))
        return;

    // Nettoyage des ennemis morts
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return !e->isAlive();
            }),
        enemies.end()
    );

    const Wave& w = waves[currentWave];
    int totalEnemies = w.simple + w.medium + w.difficult + w.hard;

    // ---------- SPAWN ----------
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
    // DEBUG MODE (Skip)
    if (debugSkipRequested) {
        std::cout << "[DEBUG] Wave skipped\n";
        enemies.clear();
        spawnedInWave = waves[currentWave].simple +
                        waves[currentWave].medium +
                        waves[currentWave].difficult +
                        waves[currentWave].hard;
        debugSkipRequested = false;
    }
    // --------------------------------------------------------------------
    // ---------- FIN DE VAGUE ----------
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

    // ---------- Ennemis normaux ----------
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

        switch (bossType) {
            case 1: {
                auto boss = std::make_unique<TralaleroTralala>(pos);
                //Death callback
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(
                    std::move(boss)
                );
                break;
            }


            case 2: {
                auto boss = std::make_unique<ChimpanziniBananini>(pos);
                //Death callback
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(
                    std::move(boss)
                );
                break;
            }


            case 3: {
                auto boss = std::make_unique<UdinDinDinDun>(pos);
                //Death callback
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(
                    std::move(boss)
                );
                break;
            }


            case 4: {
                auto boss = std::make_unique<OscarTheCrackhead>(pos);
                //Death callback
                boss->setDeathCallback([this](const Enemy& enemy) {
                    controller.spawnKeyFragmentAt(enemy.getPosition());
                });
                enemies.push_back(
                    std::move(boss)
                );
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
    if (freeTiles.empty())
        return player.getPosition();

    const float tileSize = map.getTileSize();
    const float minDist = 200.f;

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

    const auto& tile = freeTiles[rand() % freeTiles.size()];
    return {
        tile.x * tileSize + tileSize / 2.f,
        tile.y * tileSize + tileSize / 2.f
    };
}

sf::Vector2f WaveManager::getBossSpawnPosition(int& outBossType) const
{
    const float tileSize = map.getTileSize();

    if (bossTiles.empty()) {
        outBossType = 1;
        return {
            map.getWidth() * tileSize / 2.f,
            map.getHeight() * tileSize / 2.f
        };
    }

    const BossTile& bt = bossTiles[rand() % bossTiles.size()];
    outBossType = bt.bossType;

    return {
        bt.pos.x * tileSize + tileSize / 2.f,
        bt.pos.y * tileSize + tileSize / 2.f
    };
}

void WaveManager::requestSkip() {
    debugSkipRequested = true;
}
bool WaveManager::isFinished() const {
    return currentWave >= static_cast<int>(waves.size());
}

float WaveManager::getTimeBeforeFirstWave() const {
    if (firstWaveStarted) return 0.f;
    return std::max(
        0.f,
        preWaveDelay - preWaveClock.getElapsedTime().asSeconds()
    );
}