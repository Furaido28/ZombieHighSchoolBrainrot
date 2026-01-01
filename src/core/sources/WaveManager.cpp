#include "../headers/WaveManager.h"
#include "../../models/headers/Enemy.h"
#include "models/headers/Enemy/BasicEnemy.h"
#include "../../models/headers/Enemy/FastEnemy.h"
#include "models/headers/Enemy/TankEnemy.h"
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

    waveClock.restart();
}

int WaveManager::getCurrentWave() const {
    return currentWave + 1;
}

float WaveManager::getTimeLeft() const {
    float elapsed = waveClock.getElapsedTime().asSeconds();
    float remaining = maxWaveDuration - elapsed;
    return std::max(0.f, remaining);
}

void WaveManager::update(float dt, Player& player,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (!player.isAlive()) {
        waveClock.restart();
        return;
    }

    if (currentWave >= waves.size())
        return;

    const Wave& w = waves[currentWave];
    int totalEnemies = w.simple + w.medium + w.difficult + w.hard;

    // =========================
    // SPAWN DES ENNEMIS
    // =========================
    if (spawnedInWave < totalEnemies &&
        spawnClock.getElapsedTime().asSeconds() >= spawnDelay)
    {
        spawnEnemy(player, enemies);
        spawnedInWave++;
        spawnClock.restart();
    }

    // =========================
    // ENNEMIS ENCORE EN VIE ?
    // =========================
    bool enemiesAlive = false;
    for (const auto& e : enemies) {
        if (e->isAlive()) {
            enemiesAlive = true;
            break;
        }
    }

    bool allSpawned = (spawnedInWave >= totalEnemies);
    bool timeOver   = (waveClock.getElapsedTime().asSeconds() >= maxWaveDuration);

    bool nextIsLast = (currentWave == waves.size() - 2);

    // =========================
    // FIN DE VAGUE
    // =========================

    // Cas NORMAL : tout les ennemies sont mort → toujours autorisé
    if (allSpawned && !enemiesAlive) {
        currentWave++;
        spawnedInWave = 0;
        spawnClock.restart();
        waveClock.restart();
        return;
    }

    // Cas TIMER : autorisé SEULEMENT si la prochaine n’est PAS la dernière
    if (allSpawned && timeOver && !nextIsLast) {
        currentWave++;
        spawnedInWave = 0;
        spawnClock.restart();
        waveClock.restart();
        return;
    }
}


void WaveManager::spawnEnemy(Player& player,
                             std::vector<std::unique_ptr<Enemy>>& enemies)
{
    sf::Vector2f spawnPos = getSpawnPosition(player);
    const Wave& w = waves[currentWave];

    if (spawnedInWave < w.simple)
        enemies.push_back(std::make_unique<BasicEnemy>(spawnPos));
    else if (spawnedInWave < w.simple + w.medium)
        enemies.push_back(std::make_unique<FastEnemy>(spawnPos));
    else if (spawnedInWave < w.simple + w.medium + w.difficult)
        enemies.push_back(std::make_unique<BasicEnemy>(spawnPos));
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