#pragma once
#include <SFML/Graphics.hpp>
#include "../models/Player.h"
#include "../models/TileMap.h"
#include "../views/game/PlayerView.h"
// AJOUT: On inclut la vue de la map pour pouvoir l'utiliser
#include "../views/game/MapView.h"

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

private:
    Player player;
    PlayerView playerView;
    TileMap map;
    sf::View gameView;
    MapView mapView;
};