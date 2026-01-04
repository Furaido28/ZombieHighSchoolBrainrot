#include "controllers/headers/LuckyBoxController.h"
#include <iostream>

#include "controllers/headers/ItemController.h"

LuckyBoxController::LuckyBoxController(
    Player& player,
    WorldItemSystem& worldItemSystem,
    const ItemController& itemController
) : itemController(itemController),
    player(player),
    worldItemSystem(worldItemSystem)
{
    // Screamer texture
    screamerTexture.loadFromFile("assets/luckybox_popup.png");
    screamerSprite.setTexture(screamerTexture);

    // Screamer sound
    screamerSoundBuffer.loadFromFile(
        "assets/sound/sound_effect/luckybox_scream.ogg"
    );
    screamerSound.setBuffer(screamerSoundBuffer);
    screamerSound.setVolume(100.f);
}

void LuckyBoxController::update(float dt) {
    if (!screamerActive) return;

    screamerTimer -= dt;
    if (screamerTimer <= 0.f) {
        screamerActive = false;
    }
}

void LuckyBoxController::render(sf::RenderWindow& window) const {
    if (!screamerActive)
        return;

    window.setView(window.getDefaultView());

    sf::Sprite sprite = screamerSprite;

    sf::Vector2u winSize = window.getSize();
    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    float desiredCoverage = 0.8f;
    float scaleX = static_cast<float>(winSize.x) / bounds.width;
    float scaleY = static_cast<float>(winSize.y) / bounds.height;
    float scale = std::min(scaleX, scaleY) * desiredCoverage;

    sprite.setScale(scale, scale);
    sprite.setPosition(winSize.x / 2.f, winSize.y / 2.f);

    window.draw(sprite);
}

bool LuckyBoxController::isScreamerActive() const {
    return screamerActive;
}

void LuckyBoxController::triggerScreamer() {
    screamerActive = true;
    screamerTimer = screamerDuration;
    screamerSound.stop();
    screamerSound.play();
}

void LuckyBoxController::openLuckyBox(int itemIndex) {
    if (!worldItemSystem.isLuckyBox(itemIndex))
        return;

    LuckyOutcome outcome = LuckyBoxSystem::roll();

    worldItemSystem.removeItem(itemIndex);
    triggerScreamer();

    Inventory& inv = player.getInventory();

    switch (outcome) {
        case LuckyOutcome::Heal: {
            int hp = player.getHealth();
            player.setHealth(std::min(100, hp + 50));
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
            player.takeDamage(player.getHealth() / 2);
            break;
        case LuckyOutcome::LoseRandomItem:
            inv.removeRandomItem();
            break;
    }

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
    std::cout << "[LUCKYBOX] Outcome = " << outcomeToString(outcome) << std::endl;
}

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
    book.cooldown = 0.8f;
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
    deo.cooldown = 10000000.f;
    deo.isProjectile = false;
    deo.sprite.setTexture(itemController.getTexture("deo"));
    return deo;
}