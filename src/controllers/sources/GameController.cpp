#include "../headers/GameController.h"
#include "../../Constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Pour std::remove_if

#include "core/headers/LuckyBoxSystem.h"
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
// CONSTRUCTOR
// =========================
GameController::GameController() : player(), playerView(*this) {
    Inventory& inv = player.getInventory();

    // =========================
    // COMMANDES (UNE FOIS)
    // =========================
    inputHandler.bind(sf::Keyboard::Z, std::make_unique<MoveUpCommand>(player));
    inputHandler.bind(sf::Keyboard::Q, std::make_unique<MoveLeftCommand>(player));
    inputHandler.bind(sf::Keyboard::S, std::make_unique<MoveDownCommand>(player));
    inputHandler.bind(sf::Keyboard::D, std::make_unique<MoveRightCommand>(player));

    inputHandler.bind(sf::Mouse::Left, std::make_unique<AttackCommand>(player));

    inputHandler.bind(sf::Mouse::Right, std::make_unique<UseItemCommand>(player));
    inputHandler.bind(sf::Keyboard::E, std::make_unique<PickupItemCommand>(
        player,
        worldItemSystem,
        *this
        )
    );

    nextSlotCommand = std::make_unique<NextSlotCommand>(inv, tabPressed);
    prevSlotCommand = std::make_unique<PrevSlotCommand>(inv);

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
    // INIT GLOBALE (UNE FOIS)
    // =========================
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    gameView.setSize(1280.f, 720.f);
    gameView.zoom(1.0f);

    player.setSize(48.f, 48.f);

    // =========================
    // TEXTURES (UNE FOIS)
    // =========================
    itemTextures["medkit"].loadFromFile("assets/inventory_items/medkit.png");
    itemTextures["pen"].loadFromFile("assets/inventory_items/pen.png");
    itemTextures["book"].loadFromFile("assets/inventory_items/book.png");
    itemTextures["chalk"].loadFromFile("assets/inventory_items/chalk.png");
    itemTextures["key-fragment"].loadFromFile("assets/inventory_items/key.png");
    itemTextures["laptop"].loadFromFile("assets/inventory_items/laptop.png");
    itemTextures["deo"].loadFromFile("assets/inventory_items/deo.png");
    itemTextures["luckybox"].loadFromFile("assets/inventory_items/luckybox.png");

    // =========================
    // SCREAMER ASSETS
    // =========================
    screamerTexture.loadFromFile("assets/luckybox_popup.png");
    screamerSprite.setTexture(screamerTexture);
    sf::Vector2u size = screamerTexture.getSize();


    screamerSprite.setScale(0.3,0.3);

    // Son screamer
    screamerSoundBuffer.loadFromFile(
        "assets/sound/sound_effect/luckybox_scream.ogg"
    );
    screamerSound.setBuffer(screamerSoundBuffer);
    screamerSound.setVolume(100.f); // MAX SFML (très fort)

    // =========================
    // DEBUG SHAPES
    // =========================
    debugMeleeBox.setFillColor(sf::Color(255, 0, 0, 100));
    debugProjectileRange.setFillColor(sf::Color::Transparent);
    debugProjectileRange.setOutlineColor(sf::Color(0, 100, 255, 150));
    debugProjectileRange.setOutlineThickness(2.f);

    // =========================
    // PREMIER NIVEAU
    // =========================
    currentLevel = 0;
    initLevel(currentLevel);
}

// =========================ds
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
void GameController::update(float dt) {
    if(screamerActive){
        screamerTimer -= dt;
        if (screamerTimer <= 0.f) {
            screamerActive = false;
        }
    }
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
            projectileSystem.spawn(
                attack.startPosition,
                attack.velocity,
                attack.damage,
                600.f
            );
        }
        else {
            // -------- MELEE --------

            // DEBUG HITBOX
            debugMeleeBox.setPosition(
                attack.meleeHitbox.left,
                attack.meleeHitbox.top
            );
            debugMeleeBox.setSize({
                attack.meleeHitbox.width,
                attack.meleeHitbox.height
            });
            debugMeleeTimer = 0.1f;

            // DAMAGE ENEMIES
            for (auto& enemy : enemies) {
                if (!enemy->isAlive()) continue;

                if (attack.meleeHitbox.intersects(enemy->getGlobalBounds())) {
                    if (attack.damage == 0) {
                        // Cas spécial Deo
                        enemy->takeDamage(
                            static_cast<int>(enemy->getHealth() * 0.75f)
                        );
                    }
                    else {
                        enemy->takeDamage(attack.damage);
                    }
                }
            }
        }
    }

    projectileSystem.update(dt, map, enemies);

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
    if (waveManager && !levelEnding)
        waveManager->update(dt, player, enemies);

    // =========================
    // 9. CAMERA
    // =========================
    gameView.setCenter(player.getPosition());
    if (levelEnding) {
        if (levelEndClock.getElapsedTime().asSeconds() >= levelEndDuration) {
            goToNextLevel();
        }
    }
}

// =========================
// RENDER
// =========================
void GameController::render(sf::RenderWindow& window) {
    window.setView(gameView);

    window.draw(mapView);

    worldItemSystem.render(window);

    for (auto& enemy : enemies)
        enemyView.render(window, *enemy, player.getPosition());

    projectileSystem.render(window);

    if (showProjectileRange)
        window.draw(debugProjectileRange);

    if (debugMeleeTimer > 0.f)
        window.draw(debugMeleeBox);

    playerView.renderWorld(window, player);

    // =========================
    // SCREAMER RENDER (CENTERED)
    // =========================
    if (screamerActive) {
        window.setView(window.getDefaultView());

        sf::Vector2u winSize = window.getSize();

        // CORRECTION ICI : On prend les limites réelles du sprite (après le découpage IntRect)
        sf::FloatRect bounds = screamerSprite.getLocalBounds();

        // Origine au centre de la zone visible
        screamerSprite.setOrigin(
            bounds.width / 2.f,
            bounds.height / 2.f
        );

        // 🔽 FACTEUR DE TAILLE (ex: 80% de l’écran)
        float desiredCoverage = 0.8f;

        // On calcule l'échelle par rapport à la taille visible (bounds) et non la texture totale
        float scaleX = static_cast<float>(winSize.x) / bounds.width;
        float scaleY = static_cast<float>(winSize.y) / bounds.height;
        float scale = std::min(scaleX, scaleY) * desiredCoverage;

        screamerSprite.setScale(scale, scale);

        // Centrage écran
        screamerSprite.setPosition(
            winSize.x / 2.f,
            winSize.y / 2.f
        );

        window.draw(screamerSprite);
    }

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
            if (t == '#' || t=='R' || t=='B' || t=='A' || t=='r' || t=='T' || t=='~') return false;
        }
    }
    return true;
}

void GameController::placePlayerAtFirstFreeTile() {
    float tileSizeF = (float)map.getTileSize();
    bool spawnFound = false;

    // 1. On cherche d'abord le symbole '$' (Spawn Point)
    for (unsigned y = 0; y < map.getHeight(); ++y) {
        for (unsigned x = 0; x < map.getWidth(); ++x) {
            char tile = map.getTile((int)x, (int)y);

            if (tile == '$') {
                // On a trouvé le point de spawn !
                float cx = x * tileSizeF + tileSizeF / 2.f;
                float cy = y * tileSizeF + tileSizeF / 2.f;
                player.setPosition(cx, cy);
                spawnFound = true;

                // Optionnel : On peut remplacer le '$' par un sol normal '.' une fois le spawn trouvé
                // pour ne pas qu'il reste un symbole bizarre si on repasse dessus.
                // map.setTile(x, y, '.');

                std::cout << "[GAME] Player spawned at specific point ($): " << x << ", " << y << std::endl;
                return; // On arrête de chercher, on a trouvé.
            }
        }
    }

    // 2. Si aucun '$' n'est trouvé, on utilise l'ancienne méthode (premier sol libre '.')
    if (!spawnFound) {
        std::cout << "[GAME] No spawn point ($) found. Searching for first free tile..." << std::endl;
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
    }

    // 3. Sécurité ultime si la map est pleine de murs (ne devrait pas arriver)
    player.setPosition(tileSizeF + playerSize().x, tileSizeF + playerSize().y);
}

void GameController::onKeyFragmentPicked() {
    if (levelEnding) {
        return;
    }
    levelEnding = true;
    levelEndClock.restart();

    enemies.clear();      // Supprime tous les zombies instantanément
    std::cout << "[GAME] Level Complete - Enemies cleared.\n";
}
bool GameController::isLevelEnding() const {
    return levelEnding;
}
float GameController::getLevelEndRemainingTime() const {
    if (!levelEnding)
        return 0.f;
    float elapsed = levelEndClock.getElapsedTime().asSeconds();
    return std::max(0.f, levelEndDuration - elapsed);
}

static void clearInventorySlots(Inventory& inventory) {
    auto& slots = inventory.getSlots();
    for (auto& s : slots) s.reset();
    inventory.selectSlot(0);
}
void GameController::initLevel(int levelIndex) {
    // clean the game
    enemies.clear();
    worldItemSystem.clear();
    projectileSystem.clear();

    //load map
    if (!map.loadFromFile(levelMaps[levelIndex], TILE_SIZE)) {
        std::cerr << "ERROR: FAILED TO LOAD TILE MAP" << levelMaps[levelIndex]<< std::endl;
        return;
    }

    //Rebuild wave manager + map view
    waveManager = std::make_unique<WaveManager>(map, *this);
    mapView.load(map);

    //Reset Player position + inventory (except key)
    placePlayerAtFirstFreeTile();

    if (levelIndex == 0) {
        clearInventorySlots(player.getInventory());
    }

    player.setHealth(100);

    // --- 5) Respawn items ---
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemTextures["medkit"]);
    worldItemSystem.spawnRandom(map, medkit);

    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.value = 0;
    pen.damage = 15;
    pen.range = 60.f;
    pen.cooldown = 0.3f;
    pen.isProjectile = false;
    pen.sprite.setTexture(itemTextures["pen"]);
    worldItemSystem.spawnRandom(map, pen);


    if (std::rand() % 100 < 70) {
        int extraItems = 3 + std::rand() % 5;
        for (int i = 0; i < extraItems; ++i) {
            Item item;
            int r = std::rand() % 3;

            if (r == 0) {
                item.name = "Medkit";
                item.type = ItemType::Consumable;
                item.value = 60;
                item.sprite.setTexture(itemTextures["medkit"]);
            } else if (r == 1) {
                if (currentLevel == 0) {
                    item.name = "pen";
                    item.type = ItemType::Weapon;
                    item.damage = 15;
                    item.range = 60.f;
                    item.cooldown = 0.3f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemTextures["pen"]);
                }
                else {
                    item.name = "book";
                    item.type = ItemType::Weapon;
                    item.damage = 35;
                    item.range = 80.f;
                    item.cooldown = 0.8f;
                    item.isProjectile = false;
                    item.sprite.setTexture(itemTextures["book"]);
                }
            } else {
                item.name = "Chalk";
                item.type = ItemType::Weapon;
                item.damage = 10;
                item.range = 600.f;
                item.cooldown = 0.5f;
                item.isProjectile = true;
                item.projectileSpeed = 500.f;
                item.sprite.setTexture(itemTextures["chalk"]);
            }

            worldItemSystem.spawnRandom(map, item);
        }
    }

    //================
    //LUCKY BOX SPAWN
    //================

    // 1. ONE LUCKY BOX PER LEVEL
    Item luckyBox;
    luckyBox.name = "Lucky Box";
    luckyBox.type = ItemType::LuckyBox;
    luckyBox.isPickable = false;

    // 2. SPRITE
    luckyBox.sprite.setTexture(itemTextures["luckybox"]);

    // SPAWN THE BOX
    worldItemSystem.spawnRandom(map, luckyBox);

    std::cout << "[GAME] LuckyBox spawned in level " << currentLevel << std::endl;

    // --- 6) Reset camera ---
    gameView.setCenter(player.getPosition());
}
void GameController::goToNextLevel() {
    levelEnding = false;
    currentLevel++;
    if (currentLevel >= (int)levelMaps.size()) {
        currentLevel = 0;
    }
    initLevel(currentLevel);
}

void GameController::spawnKeyFragmentAt(const sf::Vector2f &pos) {
    if (levelEnding)return;
    Item fragment;
    fragment.name = "Key Fragment";
    fragment.type = ItemType::KeyFragment;
    fragment.value = 1;

    //sprite & position
    fragment.sprite.setTexture(itemTextures["key-fragment"]);
    fragment.sprite.setPosition(pos);

    worldItemSystem.spawnAt(pos, fragment);

    std::cout << "[GAME] Key Fragment spawned at boss position\n";
}

void GameController::openLuckyBox(int itemIndex) {
    if (!worldItemSystem.isLuckyBox(itemIndex))
        return;

    // Roll
    LuckyOutcome outcome = LuckyBoxSystem::roll();

    // Supprimer la box
    worldItemSystem.removeItem(itemIndex);

    // =========================
    // TRIGGER SCREAMER
    // =========================
    screamerActive = true;
    screamerTimer = screamerDuration;

    screamerSound.stop(); // sécurité
    screamerSound.play();

    // Helper to create any items
    auto makeMedkit = [&]() {
        Item medkit;
        medkit.name = "Medkit";
        medkit.type = ItemType::Consumable;
        medkit.value = 60;
        medkit.sprite.setTexture(itemTextures["medkit"]);
        return medkit;
    };

    auto makePen = [&]() {
        Item pen;
        pen.name = "Pen";
        pen.type = ItemType::Weapon;
        pen.value = 0;
        pen.damage = 15;
        pen.range = 60.f;
        pen.cooldown = 0.3f;
        pen.isProjectile = false;
        pen.sprite.setTexture(itemTextures["pen"]);
        return pen;
    };

    auto makeBook = [&]() {
        Item book;
        book.name = "Book";
        book.type = ItemType::Weapon;
        book.damage = 35;
        book.range = 90.f;
        book.cooldown = 0.8f;
        book.isProjectile = false;
        book.sprite.setTexture(itemTextures["book"]);
        return book;
    };

    auto makeChalk = [&]() {
        Item chalk;
        chalk.name = "Chalk";
        chalk.type = ItemType::Weapon;
        chalk.damage = 10;
        chalk.range = 600.f;
        chalk.cooldown = 0.5f;
        chalk.isProjectile = true;
        chalk.projectileSpeed = 500.f;
        chalk.sprite.setTexture(itemTextures["chalk"]);
        return chalk;
    };

    auto makeLaptop = [&]() {
        Item laptop;
        laptop.name = "Laptop";
        laptop.type = ItemType::Weapon;


        laptop.damage = 80;
        laptop.range = 100.f;
        laptop.cooldown = 1.2f;
        laptop.isProjectile = false;

        laptop.sprite.setTexture(itemTextures["laptop"]);
        return laptop;
    };
    auto makeDeo = [&]() {
        Item deo;
        deo.name = "Deo";
        deo.type = ItemType::Weapon;

        deo.damage = 0;
        deo.range = 150.f;
        deo.cooldown = 10000000.f;
        deo.isProjectile = false;
        deo.sprite.setTexture(itemTextures["deo"]);
        return deo;
    };

    //apply the result of the roll
    Inventory& inv = player.getInventory();

    switch (outcome) {
        case LuckyOutcome::Heal :{
            int currentHp = player.getHealth();
            int maxHp = 100;

            if(currentHp < maxHp) {
                if (rand()%100 < 50) {
                    player.setHealth(100);
                }
                else {
                    if (currentHp +50 > 100) {
                        player.setHealth(100);
                    }
                    else {
                        player.setHealth(currentHp+50);
                    }
                }
            }
            else {
                inv.addItem(makeMedkit());
            }
            std::cout << "[LUCKYBOX] Outcome : Heal\n";
            break;
        }
        case LuckyOutcome::Medkit:
            inv.addItem(makeMedkit());
            std::cout << "[LUCKYBOX] Outcome : Medkit\n";
            break;

        case LuckyOutcome::Pen:
            inv.addItem(makePen());
            std::cout << "[LuckyBox] Outcome: Pen\n";
            break;

        case LuckyOutcome::Book:
            inv.addItem(makeBook());
            std::cout << "[LuckyBox] Outcome: Book\n";
            break;

        case LuckyOutcome::Chalk:
            inv.addItem(makeChalk());
            std::cout << "[LuckyBox] Outcome: Chalk\n";
            break;

        case LuckyOutcome::Computer:
            inv.addItem(makeLaptop());
            std::cout << "[LuckyBox] Outcome: Laptop\n";
            break;
        case LuckyOutcome::Deo:
            inv.addItem(makeDeo());
            std::cout << "[LuckyBox] Outcome: Deo\n";
            break;
        case LuckyOutcome::LoseHealth:
            player.takeDamage(static_cast<int>(player.getHealth()/2));
            std::cout << "[LuckyBox] Outcome: LoseHealth\n";
            break;
        case LuckyOutcome::LoseRandomItem:
            inv.removeRandomItem();
            std::cout << "[LuckyBox] Outcome: LoseRandomItem\n";
            break;
    }
}

bool GameController::isPlayerDead() const {
    return !player.isAlive();
}