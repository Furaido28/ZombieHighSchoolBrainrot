#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "core/headers/commands/AttackCommand.h"
#include "core/headers/commands/MoveDownCommand.h"
#include "core/headers/commands/MoveLeftCommand.h"
#include "core/headers/commands/MoveRightCommand.h"
#include "core/headers/commands/MoveUpCommand.h"

static bool circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius)
{
    sf::Vector2f d = aPos - bPos;
    float distSq = d.x * d.x + d.y * d.y;
    float r = aRadius + bRadius;
    return distSq <= r * r;
}

// =========================
// RANDOM TILE HELPERS
// =========================
static sf::Vector2f getRandomFreeTileCenter(const TileMap& map) {
    std::vector<sf::Vector2i> freeTiles;

    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            if (map.getTile(x, y) == '.') {
                freeTiles.emplace_back(x, y);
            }
        }
    }

    if (freeTiles.empty())
        return {0.f, 0.f};

    int index = std::rand() % freeTiles.size();
    float tileSize = (float)map.getTileSize();

    return {
        freeTiles[index].x * tileSize + tileSize / 2.f,
        freeTiles[index].y * tileSize + tileSize / 2.f
    };
}

static void spawnWorldItem(
    std::vector<WorldItem>& worldItems,
    const TileMap& map,
    Item item)
{
    WorldItem wi;
    wi.item = item;
    wi.position = getRandomFreeTileCenter(map);
    wi.item.sprite.setPosition(wi.position);
    worldItems.push_back(wi);
}

// =========================
// CONSTRUCTOR
// =========================
GameController::GameController() : player(), playerView() {
    // =========================
    // Unitialisation des commandes
    // =========================

    inputHandler.bind(sf::Keyboard::Z,
    std::make_unique<MoveUpCommand>(player));

    inputHandler.bind(sf::Keyboard::S,
        std::make_unique<MoveDownCommand>(player));

    inputHandler.bind(sf::Keyboard::Q,
        std::make_unique<MoveLeftCommand>(player));

    inputHandler.bind(sf::Keyboard::D,
        std::make_unique<MoveRightCommand>(player));

    inputHandler.bind(sf::Mouse::Left,
    std::make_unique<AttackCommand>(
        player,
        enemies,
        attackTimer,
        attackCooldown
    )
);

    // =========================
    // LOAD MAPS
    // =========================

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "--- DEBUG: Tentative de chargement de la map ---" << std::endl;

    if (!map.loadFromFile("assets/maps/map2.txt", TILE_SIZE)) {
        std::cerr << "DEBUG: ECHEC du chargement de la map !" << std::endl;
        return;
    }

    std::cout << "DEBUG: Map chargee avec succes !" << std::endl;

    waveManager = std::make_unique<WaveManager>(map);

    mapView.load(map);

    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    player.setSize(48.f, 48.f);
    placePlayerAtFirstFreeTile();

    // =========================
    // LOAD ITEM TEXTURES
    // =========================
    itemTextures["medkit"].loadFromFile("assets/inventory_items/medkit.png");
    itemTextures["pen"].loadFromFile("assets/inventory_items/pen.png");
    itemTextures["book"].loadFromFile("assets/inventory_items/book.png");
    itemTextures["chalk"].loadFromFile("assets/inventory_items/chalk.png");

    // =========================
    // GUARANTEED ITEMS
    // =========================
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemTextures["medkit"]);
    spawnWorldItem(worldItems, map, medkit);

    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.value = 15;
    pen.sprite.setTexture(itemTextures["pen"]);
    spawnWorldItem(worldItems, map, pen);

    // =========================
    // RANDOM EXTRA ITEMS
    // =========================
    int extraItems = 3 + std::rand() % 5; // 3 à 7 items

    for (int i = 0; i < extraItems; ++i) {
        Item item;
        int r = std::rand() % 3;

        if (r == 0) {
            item.name = "Medkit";
            item.type = ItemType::Consumable;
            item.value = 60;
            item.sprite.setTexture(itemTextures["medkit"]);
        }
        else if (r == 1) {
            item.name = "Book";
            item.type = ItemType::Weapon;
            item.value = 30;
            item.sprite.setTexture(itemTextures["book"]);
        }
        else {
            item.name = "Chalk";
            item.type = ItemType::Weapon;
            item.value = 7;
            item.sprite.setTexture(itemTextures["chalk"]);
        }

        spawnWorldItem(worldItems, map, item);
    }
}

// =========================
// ACCESSORS
// =========================
Player& GameController::getPlayer() { return player; }

const sf::Texture& GameController::getItemTexture(const std::string& name) const {
    return itemTextures.at(name);
}

// =========================
// INPUT EVENTS
// =========================
void GameController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Right)
    {
        int slot = player.getInventory().getSelectedSlot();
        auto& slots = player.getInventory().getSlots();

        if (slots[slot].has_value()) {
            Item& item = slots[slot].value();

            if (item.type == ItemType::Consumable) {
                if (player.getHealth() + item.value >100)
                    player.setHealth(100);
                else
                    player.takeDamage(-item.value);
                slots[slot].reset();
            }
        }
    }
}

// =========================
// UPDATE
// =========================
void GameController::update(float dt)
{
    // =========================
    // 0. TIMERS
    // =========================
    if (attackTimer > 0.f)
        attackTimer -= dt;

    // =========================
    // 1. PLAYER INPUT
    // =========================

    // Reset mouvement
    player.setMoving(false);

    // Commandes
    inputHandler.handleInput(dt);

    // Update logique
    player.update(dt);

    // =========================
    // 2. ITEM PICKUP (E)
    // =========================
    static bool eWasPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        if (!eWasPressed) {
            for (auto it = worldItems.begin(); it != worldItems.end();) {
                if (circlesIntersect(
                    player.getPosition(), player.getRadius(),
                    it->position, it->radius))
                {
                    bool picked = false;

                    if (it->item.type == ItemType::KeyFragment)
                        picked = player.getInventory().addKeyFragment(it->item);
                    else
                        picked = player.getInventory().addItem(it->item);

                    if (picked) {
                        it = worldItems.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
        eWasPressed = true;
    }
    else {
        eWasPressed = false;
    }

    // =========================
    // 3. MOVEMENT + COLLISIONS
    // =========================
    sf::Vector2f dir = player.consumeMovement();

    if (dir.x != 0.f || dir.y != 0.f) {
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;
    }

    sf::Vector2f delta = dir * 700.f * dt;

    sf::FloatRect currentBBox = player.getGlobalBounds();
    sf::FloatRect futureBBox = currentBBox;
    futureBBox.left += delta.x;
    futureBBox.top  += delta.y;

    if (isPositionFree(futureBBox)) {
        player.move(delta);
    }
    else {
        // glissement X
        sf::FloatRect bboxX = currentBBox;
        bboxX.left += delta.x;

        if (isPositionFree(bboxX)) {
            player.move({ delta.x, 0.f });
        }
        else {
            // glissement Y
            sf::FloatRect bboxY = currentBBox;
            bboxY.top += delta.y;

            if (isPositionFree(bboxY)) {
                player.move({ 0.f, delta.y });
            }
        }
    }

    // =========================
    // 4. UPDATE PLAYER
    // =========================
    player.update(dt);

    // =========================
    // 5. ENEMY UPDATE & ATTACK
    // =========================
    for (auto& enemy : enemies) {

        if (!enemy->isAlive())
            continue;

        sf::Vector2f oldPos = enemy->getPosition();

        if (player.isAlive())
            enemy->update(dt, player.getPosition());
        else
            enemy->update(dt, enemy->getPosition());

        // =========================
        // COLLISION DÉCOR – ENNEMIS (AVEC GLISSEMENT)
        // =========================
        sf::Vector2f newPos = enemy->getPosition();
        sf::Vector2f deltaE = newPos - oldPos;

        // Revenir à la position valide
        enemy->setPosition(oldPos);

        // Tentative déplacement complet
        sf::FloatRect bbox = enemy->getGlobalBounds();
        sf::FloatRect future = bbox;
        future.left += deltaE.x;
        future.top  += deltaE.y;

        if (isPositionFree(future)) {
            enemy->setPosition(oldPos + deltaE);
        }
        else {
            // Glissement X
            sf::FloatRect bboxX = bbox;
            bboxX.left += deltaE.x;

            if (isPositionFree(bboxX)) {
                enemy->setPosition({ oldPos.x + deltaE.x, oldPos.y });
            }
            else {
                // Glissement Y
                sf::FloatRect bboxY = bbox;
                bboxY.top += deltaE.y;

                if (isPositionFree(bboxY)) {
                    enemy->setPosition({ oldPos.x, oldPos.y + deltaE.y });
                }
                else {
                    // Bloqué total → reste en place
                    enemy->setPosition(oldPos);
                }
            }
        }


        // attaque joueur
        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            enemy->getPosition(), enemy->getRadius()))
        {
            enemy->attack(player);
        }
    }

    // =========================
    // 7. DEBUG – SKIP WAVE
    // =========================
    static bool kWasPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        if (!kWasPressed && waveManager) {
            waveManager->requestSkip();
            std::cout << "[DEBUG] Skip requested\n";
        }
        kWasPressed = true;
    }
    else {
        kWasPressed = false;
    }

    // =========================
    // 8. WAVES
    // =========================
    if (waveManager)
        waveManager->update(dt, player, enemies);

    // =========================
    // 9. CAMERA
    // =========================
    gameView.setCenter(player.getPosition());
}

// =========================
// RENDER
// =========================
void GameController::render(sf::RenderWindow& window) {
    window.setView(gameView);
    window.draw(mapView);

    for (auto& wi : worldItems)
        window.draw(wi.item.sprite);

    for (auto& enemy : enemies)
        enemyView.render(window, *enemy, player.getPosition());

    playerView.renderWorld(window, player);

    window.setView(window.getDefaultView());

    int wave = waveManager ? waveManager->getCurrentWave() : 0;
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
