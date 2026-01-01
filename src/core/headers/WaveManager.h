#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include "models/headers/Enemy.h"
#include "models/headers/Enemy/BasicEnemy.h"
#include "models/headers/Enemy/FastEnemy.h"
#include "models/headers/Enemy/TankEnemy.h"

#include "models/headers/Boss/TralaleroTralala.h"
#include "models/headers/Boss/ChimpanziniBananini.h"
#include "models/headers/Boss/OscarTheCrackhead.h"

#include "models/headers/Player.h"
#include "models/headers/TileMap.h"

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <memory>

class Enemy;
class Player;
class TileMap;

struct Wave {
    int simple;
    int medium;
    int difficult;
    int hard; // boss = 1
};

class WaveManager {
public:
    explicit WaveManager(const TileMap& mapRef);

    void update(float dt, Player& player,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    int getCurrentWave() const;
    float getTimeLeft() const;

private:
    // ---------- MAP ----------
    const TileMap& map;
    std::vector<sf::Vector2u> freeTiles;   // '.'
    std::vector<sf::Vector2u> bossTiles;   // '1'

    // ---------- WAVES ----------
    std::vector<Wave> waves;
    int currentWave = 0;
    int spawnedInWave = 0;

    float spawnDelay = 1.f;
    float maxWaveDuration = 60.f;

    sf::Clock spawnClock;
    sf::Clock waveClock;

    // ---------- INTERNAL ----------
    void spawnEnemy(Player& player,
                    std::vector<std::unique_ptr<Enemy>>& enemies);

    sf::Vector2f getSpawnPosition(const Player& player);
    sf::Vector2f getBossSpawnPosition() const;
};
