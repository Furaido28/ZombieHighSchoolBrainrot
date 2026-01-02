#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include "../../models/headers/Enemy.h"
#include "models/headers/Enemy/BasicEnemy.h"
#include "../../models/headers/Enemy/FastEnemy.h"
#include "models/headers/Enemy/TankEnemy.h"
#include "../../models/headers/Boss/Boss.h"
#include "core/headers/InputHandler.h"
#include "models/headers/Item.h"
#include "views/headers/game/PlayerView.h"
#include "views/headers/game/EnemyView.h"
#include "views/headers/game/MapView.h"
#include "core/headers/WaveManager.h"

struct WorldItem {
    Item item;
    sf::Vector2f position;
    float radius = 20.f;

    sf::FloatRect getBounds() const {
        return {
            position.x - radius,
            position.y - radius,
            radius * 2.f,
            radius * 2.f
        };
    }
};

class GameController {
public:
    GameController();
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);
    Player& getPlayer();
    void givePotionToPlayer();
    const sf::Texture& getItemTexture(const std::string& name) const;

private:
    // Méthodes internes de collision
    bool isPositionFree(const sf::FloatRect& bbox) const;
    void placePlayerAtFirstFreeTile();

    // Helper: On demande la taille directement au Player (MVC)
    // Plus besoin de stocker "radius" ici, c'est le Player qui sait sa taille.
    sf::Vector2f playerSize() const { return player.getSize(); }

    //textures
    std::map<std::string, sf::Texture> itemTextures;

    Player player;
    PlayerView playerView;
    TileMap map;
    sf::View gameView;
    MapView mapView;
    std::vector<std::unique_ptr<Enemy>> enemies;
    EnemyView enemyView;

    std::unique_ptr<WaveManager> waveManager;
    std::vector<WorldItem> worldItems;
    float attackCooldown = 0.4f;
    float attackTimer = 0.f;

    //Command
    InputHandler inputHandler;
};