#pragma once
#include <SFML/Graphics.hpp>
#include "../models/Player.h"
#include "../models/GameState.h"
#include "../views/game/PlayerView.h"
#include "../models/TileMap.h"
#include "../views/game/MapView.h"

class GameController {
public:
    GameController();

    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    Player player;
    PlayerView playerView;
    GameState state;

    // --- MAP ---
    TileMap tileMap;
    MapView mapView;

    // --- AJOUT CAMÉRA ---
    sf::View gameView;
};