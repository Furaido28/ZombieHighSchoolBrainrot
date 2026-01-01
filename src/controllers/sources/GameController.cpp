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
    std::cout << "--- DEBUG: Tentative de chargement de la map ---" << std::endl;

    if (!map.loadFromFile("assets/maps/map2.txt", TILE_SIZE)) {
        std::cerr << "DEBUG: ECHEC du chargement de la map !" << std::endl;
        return;
    }

    std::cout << "DEBUG: Map chargee avec succes !" << std::endl;

    // ⚠️ WaveManager APRES le chargement de la map
    waveManager = std::make_unique<WaveManager>(map);

    mapView.load(map);

    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    player.setSize(48.f, 48.f);
    placePlayerAtFirstFreeTile();

    itemTextures["potion"].loadFromFile("assets/inventory_items/potion.png");
}

Player& GameController::getPlayer() {return player;};
const sf::Texture &GameController::getItemTexture(const std::string &name) const {
    return itemTextures.at(name);
}

void GameController::handleEvent(const sf::Event& event) {
    (void)event;
}

void GameController::update(float dt)
{
    // =========================
    // 1. INPUTS JOUEUR
    // =========================
    sf::Vector2f dir(0.f, 0.f);
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        dir.y -= 1.f;
        player.setDirection(Direction::Up);
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dir.y += 1.f;
        player.setDirection(Direction::Down);
        isMoving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        dir.x -= 1.f;
        player.setDirection(Direction::Left);
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dir.x += 1.f;
        player.setDirection(Direction::Right);
        isMoving = true;
    }

    player.setMoving(isMoving);

    // =========================
    // 2. NORMALISATION
    // =========================
    if (dir.x != 0.f || dir.y != 0.f) {
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;
    }

    const float speed = 700.f;
    sf::Vector2f delta = dir * speed * dt;

    // =========================
    // 3. COLLISIONS JOUEUR
    // =========================
    sf::FloatRect currentBBox = player.getGlobalBounds();

    sf::FloatRect futureBBox = currentBBox;
    futureBBox.left += delta.x;
    futureBBox.top  += delta.y;

    if (isPositionFree(futureBBox)) {
        player.move(delta);
    }
    else {
        sf::FloatRect bboxX = currentBBox;
        bboxX.left += delta.x;

        if (isPositionFree(bboxX)) {
            player.move({ delta.x, 0.f });
        }
        else {
            sf::FloatRect bboxY = currentBBox;
            bboxY.top += delta.y;

            if (isPositionFree(bboxY)) {
                player.move({ 0.f, delta.y });
            }
        }
    }

    // =========================
    // 4. UPDATE JOUEUR
    // =========================
    player.update(dt);

    // =========================
    // 5. UPDATE ENNEMIS
    // =========================
    for (auto& enemy : enemies) {

        if (!enemy->isAlive())
            continue;

        sf::Vector2f oldPos = enemy->getPosition();

        if (player.isAlive()) {
            enemy->update(dt, player.getPosition());
        }
        else {
            enemy->update(dt, enemy->getPosition());
        }

        if (!isPositionFree(enemy->getGlobalBounds())) {
            enemy->setPosition(oldPos);
        }
    }


    // =========================
    // 6. COLLISIONS ENNEMIS
    // =========================
    for (auto& enemy : enemies) {
        if (!enemy->isAlive())
            continue;

        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            enemy->getPosition(), enemy->getRadius()))
        {
            enemy->attack(player);
        }
    }

    // =========================
    // 7. SYSTÈME DE VAGUES
    // =========================
    if (waveManager)
        waveManager->update(dt, player, enemies);

    // =========================
    // 8. CAMÉRA
    // =========================
    gameView.setCenter(player.getPosition());
}

void GameController::render(sf::RenderWindow& window) {
    // 1. Caméra Joueur
    window.setView(gameView);
    window.draw(mapView);

    // 2. Ennemis
    sf::Vector2f playerPos = player.getPosition();

    for (auto& enemy : enemies) {
        enemyView.render(window, *enemy, playerPos);
    }

    playerView.renderWorld(window, player);

    // 3. Interface (HUD) - Remise à zéro de la caméra
    window.setView(window.getDefaultView());

    // Récupère le title de la wave
    int wave = waveManager ? waveManager->getCurrentWave() : 0;

    // Récupère le temps restant
    float timeLeft = waveManager ? waveManager->getTimeLeft() : 0.f;

    playerView.renderHUD(window, player, wave, timeLeft);
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
            if (t == '#' || t=='R' || t=='B') return false;
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
void GameController::givePotionToPlayer() {
    Item potion;
    potion.name = "Potion";
    potion.type = ItemType::Consumable;
    potion.sprite.setTexture(getItemTexture("potion"));

    player.getInventory().addItem(potion);
}