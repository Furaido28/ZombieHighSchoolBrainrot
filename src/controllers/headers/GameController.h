#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include "CombatController.h"
#include "EnemyController.h"
#include "InputController.h"
#include "ItemController.h"
#include "LuckyBoxController.h"
#include "PlayerController.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include "../../models/headers/Enemy.h"
#include "../../models/headers/Boss/Boss.h"
#include "core/headers/InputHandler.h"
#include "models/headers/Item.h"
#include "views/headers/game/EnemyView.h"
#include "views/headers/game/MapView.h"
#include "core/headers/WaveManager.h"
#include "core/headers/combat/AttackSystem.h"
#include "views/headers/game/PlayerView.h"

#include "core/headers/combat/ProjectileSystem.h"
#include "core/headers/items/WorldItemSystem.h"

class GameController {
public:
    GameController();
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

    bool isInventoryExpanded() const { return player.getInventory().isExpanded(); };

    Player& getPlayer() { return player; };
    const Player& getPlayer() const { return player; }
    PlayerView& getPlayerView() { return playerView; }

    int getWaveNumber() const { return waveManager->getCurrentWave(); };
    float getWaveTimeLeft() const { return waveManager->getTimeLeft(); };

    const sf::View& getGameView() const { return gameView; }
    void onKeyFragmentPicked();
    bool isLevelEnding() const;
    float getLevelEndRemainingTime() const;
    void spawnKeyFragmentAt(const sf::Vector2f& pos);
    WaveManager* getWaveManager(){return waveManager.get();}
    const WaveManager* getWaveManager()const {return waveManager.get();}
    void openLuckyBox(int itemIndex);

    bool isPlayerDead() const;


private:
    // NEW
    std::unique_ptr<PlayerController> playerController;
    std::unique_ptr<InputController> inputController;
    std::unique_ptr<CombatController> combatController;
    std::unique_ptr<EnemyController> enemyController;
    std::unique_ptr<ItemController> itemController;
    std::unique_ptr<LuckyBoxController> luckyBoxController;

    WorldItemSystem worldItemSystem;

    // OLD
    int currentLevel = 0;
    std::vector<std::string> levelMaps = {
        "assets/maps/map1.txt",
        "assets/maps/map2.txt",
        "assets/maps/map3.txt",
        "assets/maps/map4.txt"
    };
    void initLevel(int levelIndex);
    void goToNextLevel();
    // Méthodes internes de collision
    bool isPositionFree(const sf::FloatRect& bbox) const;
    void placePlayerAtFirstFreeTile();

    // Helper: On demande la taille directement au Player (MVC)
    // Plus besoin de stocker "radius" ici, c'est le Player qui sait sa taille.
    sf::Vector2f playerSize() const { return player.getSize(); }

    Player player;
    PlayerView playerView;
    TileMap map;
    sf::View gameView;
    MapView mapView;
    std::vector<std::unique_ptr<Enemy>> enemies;
    EnemyView enemyView;

    std::unique_ptr<WaveManager> waveManager;
    float attackCooldown = 0.4f;
    float attackTimer = 0.f;

    //Command
    bool tabPressed = false;

    InputHandler inputHandler;
    std::unique_ptr<Command> nextSlotCommand;
    std::unique_ptr<Command> prevSlotCommand;

    // --- NOUVEAU : VISUALISATION DEBUG ---
    bool levelEnding = false;
    sf::Clock levelEndClock;
    float levelEndDuration = 10.f;
};