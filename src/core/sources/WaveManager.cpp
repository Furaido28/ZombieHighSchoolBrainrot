#include "../headers/WaveManager.h"
#include "models/headers/Enemy.h"
#include "models/headers/ZombieBasic.h"
#include "models/headers/ZombieFast.h"
#include "models/headers/ZombieTank.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include <cstdlib>

WaveManager::WaveManager(const TileMap& mapRef)
    : map(mapRef)
{
    // Pré-calcul des tuiles libres '.'
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            if (map.getTile(x, y) == '.') {
                freeTiles.emplace_back(x, y);
            }
        }
    }

    waves = {
        {6, 0, 0, 0},
        {10, 4, 0, 0},
        {14, 8, 3, 0},
        {20, 8, 6, 1},
    };
}

int WaveManager::getCurrentWave() const {
    return currentWave + 1;
}

void WaveManager::update(float dt, Player& player,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (currentWave >= waves.size()) return;

    const Wave& w = waves[currentWave];
    int totalEnemies = w.simple + w.medium + w.difficult + w.hard;

    if (spawnedInWave >= totalEnemies) {
        for (const auto& e : enemies)
            if (e->isAlive())
                return;

        currentWave++;
        spawnedInWave = 0;
        spawnClock.restart();
        return;
    }

    if (spawnClock.getElapsedTime().asSeconds() >= spawnDelay) {
        spawnEnemy(player, enemies);
        spawnedInWave++;
        spawnClock.restart();
    }
}

void WaveManager::spawnEnemy(Player& player,
                             std::vector<std::unique_ptr<Enemy>>& enemies)
{
    sf::Vector2f spawnPos = getSpawnPosition(player);
    const Wave& w = waves[currentWave];

    if (spawnedInWave < w.simple)
        enemies.push_back(std::make_unique<ZombieBasic>(spawnPos));
    else if (spawnedInWave < w.simple + w.medium)
        enemies.push_back(std::make_unique<ZombieFast>(spawnPos));
    else if (spawnedInWave < w.simple + w.medium + w.difficult)
        enemies.push_back(std::make_unique<ZombieTank>(spawnPos));
}

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