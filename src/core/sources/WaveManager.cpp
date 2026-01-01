#include "../headers/WaveManager.h"

#include "models/headers/Boss/UdinDinDinDun.h"

// ======================================================
// CONSTRUCTOR
// ======================================================
WaveManager::WaveManager(const TileMap& mapRef)
    : map(mapRef)
{
    // ---------- Scan map ----------
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile(x, y);

            if (tile == '.')
                freeTiles.emplace_back(x, y);
            else if (tile == '*')
                bossTiles.emplace_back(x, y);
        }
    }

    // ---------- 3 cycles × 4 waves ----------
    for (int cycle = 0; cycle < 3; ++cycle) {
        waves.push_back({0,0,0,1});
        // waves.push_back({6, 0, 0, 0});
        // waves.push_back({10, 4, 0, 0});
        // waves.push_back({14, 8, 3, 0});
        // waves.push_back({20, 8, 6, 1});
    }

    spawnClock.restart();
    waveClock.restart();
}

// ======================================================
// GETTERS
// ======================================================
int WaveManager::getCurrentWave() const {
    return currentWave + 1;
}

float WaveManager::getTimeLeft() const {
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

    int waveInCycle = currentWave % 4;
    int cycleIndex  = currentWave / 4;

    // ---------- Ennemis normaux ----------
    if (index < w.simple) {
        enemies.push_back(
            std::make_unique<BasicEnemy>(getSpawnPosition(player))
        );
    }
    else if (index < w.simple + w.medium) {
        enemies.push_back(
            std::make_unique<FastEnemy>(getSpawnPosition(player))
        );
    }
    else if (index < w.simple + w.medium + w.difficult) {
        enemies.push_back(
            std::make_unique<TankEnemy>(getSpawnPosition(player))
        );
    }
    // ---------- BOSS ----------
    else if (waveInCycle == 3 && w.hard > 0) {

        sf::Vector2f pos = getBossSpawnPosition();

        switch (cycleIndex) {
            case 0:
                enemies.push_back(
                    std::make_unique<TralaleroTralala>(pos)
                );
                break;

            case 1:
                enemies.push_back(
                    std::make_unique<ChimpanziniBananini>(pos)
                );
                break;

            case 2:
                enemies.push_back(
                    std::make_unique<UdinDinDinDun>(pos)
                );
                break;
            case 3:
                enemies.push_back(
                    std::make_unique<OscarTheCrackhead>(pos)
                );
                break;
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

sf::Vector2f WaveManager::getBossSpawnPosition() const
{
    const float tileSize = map.getTileSize();

    if (bossTiles.empty()) {
        return {
            map.getWidth() * tileSize / 2.f,
            map.getHeight() * tileSize / 2.f
        };
    }

    const auto& tile = bossTiles[rand() % bossTiles.size()];

    return {
        tile.x * tileSize + tileSize / 2.f,
        tile.y * tileSize + tileSize / 2.f
    };
}
