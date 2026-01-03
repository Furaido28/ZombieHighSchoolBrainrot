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
struct BossTile {
    sf::Vector2i pos;
    int bossType; // 1,2,3,4
};

class WaveManager {
public:
    explicit WaveManager(const TileMap& mapRef, GameController& controller);

    void update(float dt, Player& player,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    int getCurrentWave() const;
    float getTimeLeft() const;

    // ---------- KEY PASS WAVES ----------
    // TODO retirer plus tard
    bool debugSkipRequested = false;
    void requestSkip();
    bool isFinished() const;

private:
    // ---------- MAP ----------
    const TileMap& map;
    std::vector<sf::Vector2u> freeTiles;
    std::vector<BossTile> bossTiles;

    // ---------- WAVES ----------
    std::vector<Wave> waves;
    int currentWave = 0;
    int spawnedInWave = 0;

    float spawnDelay = 1.f;
    float maxWaveDuration = 150.f;

    sf::Clock spawnClock;
    sf::Clock waveClock;

    // ---------- INTERNAL ----------
    void spawnEnemy(Player& player,
                    std::vector<std::unique_ptr<Enemy>>& enemies);

    sf::Vector2f getSpawnPosition(const Player& player);
    sf::Vector2f getBossSpawnPosition(int &outBossType) const;
    EnemyFlyweightFactory enemyFactory;
    GameController& controller;
};
