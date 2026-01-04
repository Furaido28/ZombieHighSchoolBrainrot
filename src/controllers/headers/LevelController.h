#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

// Forward declarations pour éviter les includes lourds dans le .h
class TileMap;
class Player;
class Enemy;
class WorldItemSystem;
class ItemController;
class WaveManager;

class LevelController {
public:
    // On passe tout ce dont il a besoin pour travailler par référence ou pointeur
    LevelController(
        TileMap& map, 
        Player& player, 
        std::vector<std::unique_ptr<Enemy>>& enemies, 
        WorldItemSystem& worldItemSystem, 
        ItemController& itemController,
        std::unique_ptr<WaveManager>& waveManagerPtr // Ref vers le pointeur unique pour pouvoir le reset
    );

    void update(float dt);

    // Charge un niveau spécifique (logique du $)
    void loadLevel(int levelIndex);

    // Passe au niveau suivant
    void nextLevel();

    // Gestion de la fin de niveau (clé ramassée)
    void triggerLevelEnd();
    bool isLevelEnding() const;
    float getTimeRemaining() const;
    void spawnKeyFragment(const sf::Vector2f& position);

    // Getters utiles
    int getCurrentLevel() const { return currentLevel; }

    // Initialisation (spawn $, items)
    void placePlayerAtSpawn();
    void spawnInitialItems();
    void spawnLuckyBox();

private:
    // Références vers les données du GameController
    TileMap& map;
    Player& player;
    std::vector<std::unique_ptr<Enemy>>& enemies;
    WorldItemSystem& worldItemSystem;
    ItemController& itemController;
    std::unique_ptr<WaveManager>& waveManager; // On garde une ref vers le unique_ptr du GC

    // Données internes du LevelController
    std::vector<std::string> levelMaps;
    int currentLevel;

    // Gestion fin de niveau
    bool levelEnding;
    sf::Clock levelEndClock;
    float levelEndDuration;
    
    // Délai début de vague (warmup)
    float waveSpawnDelay; 
};