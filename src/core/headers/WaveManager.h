#pragma once

#include <SFML/System.hpp>
#include <vector>
#include <memory>

class Enemy;
class Player;
class TileMap;

struct Wave {
    int simple;
    int medium;
    int difficult;
    int hard;
};

class WaveManager {
public:
    WaveManager(const TileMap& map);

    void update(float dt, Player& player,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    int getCurrentWave() const;

private:
    void spawnEnemy(Player& player,
                    std::vector<std::unique_ptr<Enemy>>& enemies);

    sf::Vector2f getSpawnPosition(const Player& player);

    const TileMap& map;
    std::vector<sf::Vector2i> freeTiles;

    std::vector<Wave> waves;
    int currentWave = 0;
    int spawnedInWave = 0;

    sf::Clock spawnClock;
    float spawnDelay = 1.0f;
};
