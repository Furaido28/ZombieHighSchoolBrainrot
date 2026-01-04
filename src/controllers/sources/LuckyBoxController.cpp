/* ==========================================================
 * Includes
 * ========================================================== */
#include "controllers/headers/LuckyBoxController.h"
#include <iostream>

#include "controllers/headers/ItemController.h"

/* ==========================================================
 * LuckyBoxController Constructor
 * Initializes screamer visuals and audio
 * ========================================================== */
LuckyBoxController::LuckyBoxController(
    Player& player,
    WorldItemSystem& worldItemSystem,
    const ItemController& itemController
) : itemController(itemController),
    player(player),
    worldItemSystem(worldItemSystem)
{
    /* =========================
     * Screamer Visuals
     * ========================= */

    // Load screamer texture
    screamerTexture.loadFromFile("assets/luckybox_popup.png");
    screamerSprite.setTexture(screamerTexture);

    /* =========================
     * Screamer Audio
     * ========================= */

    // Load and configure screamer sound
    screamerSoundBuffer.loadFromFile(
        "assets/sound/sound_effect/luckybox_scream.ogg"
    );
    screamerSound.setBuffer(screamerSoundBuffer);
    screamerSound.setVolume(100.f);
}

/* ==========================================================
 * LuckyBoxController::update
 * Updates screamer timer and state
 * ========================================================== */
void LuckyBoxController::update(float dt) {

    // No update if screamer is inactive
    if (!screamerActive) return;

    // Decrease screamer timer
    screamerTimer -= dt;

    // Disable screamer when duration is over
    if (screamerTimer <= 0.f) {
        screamerActive = false;
    }
}

/* ==========================================================
 * LuckyBoxController::render
 * Renders screamer overlay on screen
 * ========================================================== */
void LuckyBoxController::render(sf::RenderWindow& window) const {

    // Do not render if screamer is inactive
    if (!screamerActive)
        return;

    // Use default view (UI overlay)
    window.setView(window.getDefaultView());

    // Copy sprite to safely apply transformations
    sf::Sprite sprite = screamerSprite;

    // Window size
    sf::Vector2u winSize = window.getSize();

    // Sprite bounds
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Center sprite origin
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Desired screen coverage (80%)
    float desiredCoverage = 0.8f;

    // Compute scaling factor
    float scaleX = static_cast<float>(winSize.x) / bounds.width;
    float scaleY = static_cast<float>(winSize.y) / bounds.height;
    float scale = std::min(scaleX, scaleY) * desiredCoverage;

    // Apply scale and center position
    sprite.setScale(scale, scale);
    sprite.setPosition(winSize.x / 2.f, winSize.y / 2.f);

    // Draw screamer sprite
    window.draw(sprite);
}

/* ==========================================================
 * Screamer State Accessors
 * ========================================================== */
bool LuckyBoxController::isScreamerActive() const {
    return screamerActive;
}

/* ==========================================================
 * LuckyBoxController::triggerScreamer
 * Activates screamer visual and audio
 * ========================================================== */
void LuckyBoxController::triggerScreamer() {

    // Activate screamer
    screamerActive = true;
    screamerTimer = screamerDuration;

    // Restart screamer sound
    screamerSound.stop();
    screamerSound.play();
}

/* ==========================================================
 * LuckyBoxController::openLuckyBox
 * Handles lucky box opening and outcome resolution
 * ========================================================== */
void LuckyBoxController::openLuckyBox(int itemIndex) {

    // Safety check: ensure item is a lucky box
    if (!worldItemSystem.isLuckyBox(itemIndex))
        return;

    // Roll a random lucky box outcome
    LuckyOutcome outcome = LuckyBoxSystem::roll();

    // Remove lucky box from the world
    worldItemSystem.removeItem(itemIndex);

    // Trigger screamer effect
    triggerScreamer();

    // Player inventory reference
    Inventory& inv = player.getInventory();

    /* =========================
     * Outcome Resolution
     * ========================= */
    switch (outcome) {

        case LuckyOutcome::Heal: {
            // Heal the player (max 100 HP)
            int hp = player.getHealth();
            player.setHealth(std::min(250, hp + 100));
            break;
        }

        case LuckyOutcome::Medkit:
            inv.addItem(makeMedkit());
            break;

        case LuckyOutcome::Pen:
            inv.addItem(makePen());
            break;

        case LuckyOutcome::Book:
            inv.addItem(makeBook());
            break;

        case LuckyOutcome::Chalk:
            inv.addItem(makeChalk());
            break;

        case LuckyOutcome::Computer:
            inv.addItem(makeLaptop());
            break;

        case LuckyOutcome::Deo:
            inv.addItem(makeDeo());
            break;

        case LuckyOutcome::LoseHealth:
            // Player loses half of current health
            player.takeDamage(player.getHealth() / 2);
            break;

        case LuckyOutcome::LoseRandomItem:
            // Remove a random item from inventory
            inv.removeRandomItem();
            break;
    }

    /* =========================
     * Debug Output
     * ========================= */

    // Convert outcome enum to string
    auto outcomeToString = [](LuckyOutcome o) {
        switch (o) {
            case LuckyOutcome::Heal:           return "Heal";
            case LuckyOutcome::Medkit:         return "Medkit";
            case LuckyOutcome::Pen:            return "Pen";
            case LuckyOutcome::Book:           return "Book";
            case LuckyOutcome::Chalk:          return "Chalk";
            case LuckyOutcome::Computer:       return "Laptop";
            case LuckyOutcome::Deo:            return "Deo";
            case LuckyOutcome::LoseHealth:     return "LoseHealth";
            case LuckyOutcome::LoseRandomItem: return "LoseRandomItem";
            default:                           return "Unknown";
        }
    };

    std::cout << "[LUCKYBOX] Opened\n";
    std::cout << "[LUCKYBOX] Outcome = " 
              << outcomeToString(outcome) << std::endl;
}

/* ==========================================================
 * Item Factory Methods
 * ========================================================== */
Item LuckyBoxController::makeMedkit() const {
    Item medkit;
    medkit.name = "Medkit";
    medkit.type = ItemType::Consumable;
    medkit.value = 60;
    medkit.sprite.setTexture(itemController.getTexture("medkit"));
    return medkit;
}

Item LuckyBoxController::makePen() const {
    Item pen;
    pen.name = "Pen";
    pen.type = ItemType::Weapon;
    pen.damage = 15;
    pen.range = 60.f;
    pen.cooldown = 0.3f;
    pen.isProjectile = false;
    pen.sprite.setTexture(itemController.getTexture("pen"));
    return pen;
}

Item LuckyBoxController::makeBook() const {
    Item book;
    book.name = "Book";
    book.type = ItemType::Weapon;
    book.damage = 35;
    book.range = 90.f;
    book.cooldown = 0.65f;
    book.isProjectile = false;
    book.sprite.setTexture(itemController.getTexture("book"));
    return book;
}

Item LuckyBoxController::makeChalk() const {
    Item chalk;
    chalk.name = "Chalk";
    chalk.type = ItemType::Weapon;
    chalk.damage = 10;
    chalk.range = 600.f;
    chalk.cooldown = 0.5f;
    chalk.isProjectile = true;
    chalk.projectileSpeed = 500.f;
    chalk.sprite.setTexture(itemController.getTexture("chalk"));
    return chalk;
}

Item LuckyBoxController::makeLaptop() const {
    Item laptop;
    laptop.name = "Laptop";
    laptop.type = ItemType::Weapon;
    laptop.damage = 80;
    laptop.range = 100.f;
    laptop.cooldown = 1.2f;
    laptop.isProjectile = false;
    laptop.sprite.setTexture(itemController.getTexture("laptop"));
    return laptop;
}

Item LuckyBoxController::makeDeo() const {
    Item deo;
    deo.name = "Deo";
    deo.type = ItemType::Weapon;
    deo.damage = 0;
    deo.range = 150.f;
    deo.cooldown = 2.f;
    deo.isProjectile = false;
    deo.sprite.setTexture(itemController.getTexture("deo"));
    return deo;
}
