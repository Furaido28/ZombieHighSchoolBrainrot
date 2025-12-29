#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>

static bool circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius)
{
    sf::Vector2f d = aPos - bPos;
    float distSq = d.x * d.x + d.y * d.y;
    float r = aRadius + bRadius;
    return distSq <= r * r;
}

GameController::GameController()
: player(), playerView()
{
    // --- CHARGEMENT DE LA MAP ---
    std::cout << "--- DEBUG: Tentative de chargement de la map ---" << std::endl;

    // On charge une seule fois (nettoyage du doublon)
    if (map.loadFromFile("assets/maps/map1.txt", TILE_SIZE)) {
        std::cout << "DEBUG: Map chargee avec succes !" << std::endl;
        std::cout << "DEBUG: Taille de la map: " << map.getWidth() << " x " << map.getHeight() << std::endl;
    } else {
        std::cerr << "DEBUG: ECHEC du chargement de la map ! Verifiez le Working Directory." << std::endl;
    }

    // Configure la vue de la map
    mapView.load(map);

    // Configuration de la caméra
    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    // Initialisation du joueur
    player.setSize(48.f, 48.f);
    placePlayerAtFirstFreeTile();

    // Initialisation de quelques zombies
    enemies.push_back(std::make_unique<ZombieBasic>(sf::Vector2f(100.f, 100.f)));
    enemies.push_back(std::make_unique<ZombieFast>(sf::Vector2f(200.f, 100.f)));
    enemies.push_back(std::make_unique<ZombieTank>(sf::Vector2f(300.f, 100.f)));
    enemies.push_back(std::make_unique<Boss>(sf::Vector2f(500.f, 300.f)));

}

void GameController::handleEvent(const sf::Event& event) {
    (void)event;
}

void GameController::update(float dt) {
    sf::Vector2f dir(0.f, 0.f);

    bool isMoving = false;

    // --- INPUTS & ANIMATION ---

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        dir.y -= 1.f;
        player.setDirection(Direction::Up); // DOS
        isMoving = true; // On bouge
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dir.y += 1.f;
        player.setDirection(Direction::Down); // FACE
        isMoving = true; // On bouge !
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        dir.x -= 1.f;
        player.setDirection(Direction::Left); // GAUCHE
        isMoving = true; // On bouge !
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dir.x += 1.f;
        player.setDirection(Direction::Right); // DROITE
        isMoving = true; // On bouge !
    }

    // IMPORTANT : On envoie l'info au modèle Player
    // Sans ça, l'animation reste figée
    player.setMoving(isMoving);

    // --- NORMALISATION ---
    if (dir.x != 0.f || dir.y != 0.f) {
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        dir /= len;
    }

    const float speed = 200.f;
    sf::Vector2f delta = dir * (speed * dt);

    // --- COLLISIONS ---
    sf::FloatRect currentBBox = player.getGlobalBounds();

    // 1. Test Position Future (Diagonale)
    sf::FloatRect futureBBox = currentBBox;
    futureBBox.left += delta.x;
    futureBBox.top += delta.y;

    if (isPositionFree(futureBBox)) {
        player.move(delta);
    } else {
        // 2. Glissement (Sliding) sur X
        sf::FloatRect bboxX = currentBBox;
        bboxX.left += delta.x;
        if (isPositionFree(bboxX)) {
            player.move({delta.x, 0.f});
        }
        // 3. Glissement (Sliding) sur Y
        else {
            sf::FloatRect bboxY = currentBBox;
            bboxY.top += delta.y;
            if (isPositionFree(bboxY)) {
                player.move({0.f, delta.y});
            }
        }
    }

    // Mise à jour finale du joueur
    player.update(dt);

    // UPDATE DES ENNEMIS
    for (auto& enemy : enemies) {
        enemy->update(dt, player.getPosition());
    }

    // Enemy collisions
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        if (circlesIntersect(
                player.getPosition(), player.getRadius(),
                enemy->getPosition(), enemy->getRadius()))
        {
            //player.takeDamage(enemy->getDamage());
            std::cout << "COLLISION!" << std::endl;
        }
    }

    // Caméra suit le joueur
    gameView.setCenter(player.getPosition());
}

void GameController::render(sf::RenderWindow& window) {
    // 1. Caméra Joueur
    window.setView(gameView);
    window.draw(mapView);

    // 2. Ennemis
    for (auto& enemy : enemies) {
        enemyView.render(window, *enemy);
    }

    playerView.render(window, player);

    // 3. Interface (HUD) - Remise à zéro de la caméra
    window.setView(window.getDefaultView());
}

bool GameController::isPositionFree(const sf::FloatRect& bbox) const {
    float tileSizeF = (float)map.getTileSize();

    int x0 = (int)std::floor(bbox.left / tileSizeF);
    int y0 = (int)std::floor(bbox.top / tileSizeF);
    int x1 = (int)std::floor((bbox.left + bbox.width - 0.001f) / tileSizeF);
    int y1 = (int)std::floor((bbox.top + bbox.height - 0.001f) / tileSizeF);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char t = map.getTile(x, y);
            if (t == '#') return false;
        }
    }
    return true;
}

void GameController::placePlayerAtFirstFreeTile() {
    float tileSizeF = (float)map.getTileSize();
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            if (map.getTile((int)x, (int)y) == '.') {
                float cx = x * tileSizeF + tileSizeF / 2.f;
                float cy = y * tileSizeF + tileSizeF / 2.f;
                player.setPosition(cx, cy);
                return;
            }
        }
    }
    player.setPosition(tileSizeF + playerSize().x, tileSizeF + playerSize().y);
}