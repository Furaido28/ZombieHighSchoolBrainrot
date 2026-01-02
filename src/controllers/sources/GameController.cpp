#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Pour std::remove_if

#include "core/headers/commands/AttackCommand.h"
#include "core/headers/commands/MoveDownCommand.h"
#include "core/headers/commands/MoveLeftCommand.h"
#include "core/headers/commands/MoveRightCommand.h"
#include "core/headers/commands/MoveUpCommand.h"
#include "core/headers/commands/NextSlotCommand.h"
#include "core/headers/commands/PickupItemCommand.h"
#include "core/headers/commands/PrevSlotCommand.h"
#include "core/headers/commands/SelectSlotCommand.h"
#include "core/headers/commands/SetInventoryExpandedCommand.h"
#include "core/headers/commands/UseItemCommand.h"

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
    Inventory& inv = player.getInventory();

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

    inputHandler.bind(sf::Mouse::Right,
        std::make_unique<UseItemCommand>(player)
    );

    inputHandler.bind(sf::Keyboard::E,
        std::make_unique<PickupItemCommand>(
            player,
            worldItems
        )
    );

    nextSlotCommand = std::make_unique<NextSlotCommand>(inv, tabPressed);
    prevSlotCommand = std::make_unique<PrevSlotCommand>(inv);

    // Select Slot Command
    inputHandler.bind(sf::Keyboard::Num1, std::make_unique<SelectSlotCommand>(inv, 0));
    inputHandler.bind(sf::Keyboard::Num2, std::make_unique<SelectSlotCommand>(inv, 1));
    inputHandler.bind(sf::Keyboard::Num3, std::make_unique<SelectSlotCommand>(inv, 2));
    inputHandler.bind(sf::Keyboard::Num4, std::make_unique<SelectSlotCommand>(inv, 3));
    inputHandler.bind(sf::Keyboard::Num5, std::make_unique<SelectSlotCommand>(inv, 4));
    inputHandler.bind(sf::Keyboard::Num6, std::make_unique<SelectSlotCommand>(inv, 5));
    inputHandler.bind(sf::Keyboard::Num7, std::make_unique<SelectSlotCommand>(inv, 6));
    inputHandler.bind(sf::Keyboard::Num8, std::make_unique<SelectSlotCommand>(inv, 7));
    inputHandler.bind(sf::Keyboard::Num9, std::make_unique<SelectSlotCommand>(inv, 8));

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

    // --- CONFIGURATION DU STYLO (Melee rapide) ---
    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.value = 0;
    pen.damage = 15;
    pen.range = 60.f;
    pen.cooldown = 0.3f;
    pen.isProjectile = false;
    pen.sprite.setTexture(itemTextures["pen"]);
    spawnWorldItem(worldItems, map, pen);

    // =========================
    // RANDOM EXTRA ITEMS
    // =========================
    int extraItems = 3 + std::rand() % 5;

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
            // --- LIVRE (Melee lente mais zone large) ---
            item.name = "Book";
            item.type = ItemType::Weapon;
            item.damage = 35;
            item.range = 90.f;
            item.cooldown = 0.8f;
            item.isProjectile = false;
            item.sprite.setTexture(itemTextures["book"]);
        }
        else {
            // --- CRAIE (Projectile) ---
            item.name = "Chalk";
            item.type = ItemType::Weapon;
            item.damage = 10;
            item.range = 600.f;
            item.cooldown = 0.5f;
            item.isProjectile = true;
            item.projectileSpeed = 500.f;
            item.sprite.setTexture(itemTextures["chalk"]);
        }

        spawnWorldItem(worldItems, map, item);
    }

    // --- NOUVEAU : Initialisation des formes de debug ---
    debugMeleeBox.setFillColor(sf::Color(255, 0, 0, 100)); // Rouge semi-transparent

    debugProjectileRange.setFillColor(sf::Color::Transparent);
    debugProjectileRange.setOutlineColor(sf::Color(0, 100, 255, 150)); // Bleu
    debugProjectileRange.setOutlineThickness(2.f);
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
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Tab)
    {
        tabPressed = true;
    }

    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Tab)
    {
        tabPressed = false;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        player.requestAttack();
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            nextSlotCommand->execute(0.f);
        else
            prevSlotCommand->execute(0.f);
    }

    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Tab)
    {
        tabPressed = false;
    }
}

// =========================
// UPDATE
// =========================
void GameController::update(float dt)
{
    // --- NOUVEAU : Gestion du timer de debug ---
    if (debugMeleeTimer > 0.f)
        debugMeleeTimer -= dt;

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
    // 3. MOVEMENT + COLLISIONS
    // =========================

    sf::Vector2f dir = player.consumeMovement();

    if (dir.x != 0.f || dir.y != 0.f) {
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;
    }

    sf::Vector2f delta = dir * 300.f * dt;

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

    // --- NOUVEAU : Mise à jour de la visualisation de portée (Projectile) ---
    showProjectileRange = false;
    int slot = player.getInventory().getSelectedSlot();
    auto& slots = player.getInventory().getSlots();

    if (slot >= 0 && slot < (int)slots.size() && slots[slot].has_value()) {
        const Item& equippedItem = slots[slot].value();
        if (equippedItem.type == ItemType::Weapon && equippedItem.isProjectile) {
            showProjectileRange = true;
            float range = equippedItem.range;
            debugProjectileRange.setRadius(range);
            debugProjectileRange.setOrigin(range, range);
            debugProjectileRange.setPosition(player.getPosition());
        }
    }

    // =========================
    // 5. ENEMY UPDATE & ATTACK
    // =========================

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        sf::Vector2f oldPos = enemy->getPosition();

        if (player.isAlive())
            enemy->update(dt, player.getPosition());
        else
            enemy->update(dt, enemy->getPosition());

        // Collision Enemy vs Map
        sf::Vector2f newPos = enemy->getPosition();
        sf::Vector2f deltaE = newPos - oldPos;
        enemy->setPosition(oldPos); // Reset

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

        // Enemy Attack Player
        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            enemy->getPosition(), enemy->getRadius()))
        {
            enemy->attack(player);
        }
    }

    // =========================
    // 6. PLAYER ATTACK
    // =========================
    AttackInfo attack = player.tryAttack();

    if (attack.valid) {

        if (attack.isProjectile) {
            // -------- PROJECTILE --------
            Projectile p;
            p.position = attack.startPosition;
            p.velocity = attack.velocity;
            p.damage = attack.damage;
            p.distanceTraveled = 0.f;
            p.maxRange = 600.f; // ou item.range si tu veux
            p.active = true;

            p.shape.setRadius(4.f);
            p.shape.setOrigin(4.f, 4.f);
            p.shape.setFillColor(sf::Color::White);
            p.shape.setPosition(p.position);

            projectiles.push_back(p);
        }
        else {
            // -------- MELEE --------
            debugMeleeBox.setPosition(
                attack.meleeHitbox.left,
                attack.meleeHitbox.top
            );
            debugMeleeBox.setSize({
                attack.meleeHitbox.width,
                attack.meleeHitbox.height
            });
            debugMeleeTimer = 0.1f;

            for (auto& enemy : enemies) {
                if (!enemy->isAlive()) continue;

                if (attack.meleeHitbox.intersects(enemy->getGlobalBounds())) {
                    enemy->takeDamage(attack.damage);
                }
            }
        }
    }

    // ==========================================
    // 6.5 PROJECTILE UPDATE LOOP
    // ==========================================
    for (auto& p : projectiles) {
        if (!p.active) continue;

        // Déplacement
        sf::Vector2f moveAmount = p.velocity * dt;
        p.position += moveAmount;
        p.distanceTraveled += std::sqrt(moveAmount.x * moveAmount.x + moveAmount.y * moveAmount.y);
        p.shape.setPosition(p.position);

        // 1. Collision Mur
        sf::FloatRect pBox(p.position.x - 2, p.position.y - 2, 4, 4);
        if (!isPositionFree(pBox)) {
            p.active = false;
            continue;
        }

        // 2. Portée Max
        if (p.distanceTraveled >= p.maxRange) {
            p.active = false;
            continue;
        }

        // 3. Collision Ennemis
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            if (enemy->getGlobalBounds().contains(p.position)) {
                enemy->takeDamage(p.damage);
                p.active = false; // Détruit le projectile
                break;
            }
        }
    }

    // Nettoyage des projectiles inactifs
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p) { return !p.active; }), projectiles.end());


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

    // Dessin des ennemis
    for (auto& enemy : enemies)
        enemyView.render(window, *enemy, player.getPosition());

    // --- DESSIN DES PROJECTILES ---
    for (const auto& p : projectiles) {
        window.draw(p.shape);
    }

    // --- NOUVEAU : DESSIN DEBUG ---
    if (showProjectileRange) {
        window.draw(debugProjectileRange);
    }
    if (debugMeleeTimer > 0.f) {
        window.draw(debugMeleeBox);
    }

    playerView.renderWorld(window, player);

    window.setView(window.getDefaultView());

    int wave = waveManager ? waveManager->getCurrentWave() : 0;
    float timeLeft = waveManager ? waveManager->getTimeLeft() : 0.f;

    playerView.renderHUD(window, player, wave, timeLeft);
}

// =========================
// UTILS
// =========================
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
