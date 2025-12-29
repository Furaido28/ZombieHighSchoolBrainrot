#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../../models/headers/Player.h"
#include "../../models/headers/TileMap.h"
#include "../../models/headers/Enemy.h"
#include "../../models/headers/ZombieBasic.h"
#include "../../models/headers/ZombieFast.h"
#include "../../models/headers/ZombieTank.h"
#include "../../models/headers/Boss.h"
#include "../../views/headers/game/PlayerView.h"
#include "../../views/headers/game/EnemyView.h"
// AJOUT: On inclut la vue de la map pour pouvoir l'utiliser
#include "../../views/headers/game/MapView.h"

class GameController {
public:
    GameController();
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
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
};