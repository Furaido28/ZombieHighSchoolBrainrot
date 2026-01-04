#include "../headers/Player.h"
#include <algorithm> // For std::max

// Constructor: initialize player default values
Player::Player()
    : position(0.f, 0.f),
      size(48.f, 48.f),
      currentDirection(Direction::Down),
      moving(false),
      attackCooldownTimer(0.f) // Attack cooldown starts ready
{
    maxHealth = 250;
    health = maxHealth;
}

// Applies damage to the player
void Player::takeDamage(int amount){
    // Ignore damage if player is invincible
    if (invincibilityTimer > 0.f)
        return;

    health -= amount;
    if (health < 0)
        health = 0;

    // Start invincibility frames
    invincibilityTimer = 1.5f;
}

// Returns current health
int Player::getHealth() const {
    return health;
}

// Returns maximum health
int Player::getMaxHealth() const {
    return maxHealth;
}

// Sets health directly (use carefully)
void Player::setHealth(int value) {
    health = value;
}

// Checks if the player is currently invincible
bool Player::isInvincible() const{
    return invincibilityTimer > 0.f;
}

// Checks if the player is alive
bool Player::isAlive() const {
    return health > 0;
}

// Moves the player by a delta vector
void Player::move(const sf::Vector2f& delta) {
    position += delta;
}

// Sets the absolute position
void Player::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
}

// Sets player size
void Player::setSize(float w, float h) {
    size = sf::Vector2f(w, h);
}

// Update timers (invincibility and attack cooldown)
void Player::update(float dt) {
    if (invincibilityTimer > 0.f)
        invincibilityTimer -= dt;

    if (attackCooldownTimer > 0.f)
        attackCooldownTimer -= dt;
}

// Returns player position
sf::Vector2f Player::getPosition() const {
    return position;
}

// Returns player size
sf::Vector2f Player::getSize() const {
    return size;
}

// Returns collision radius (used for circular checks)
float Player::getRadius() const {
    return std::min(size.x, size.y) * 0.5f;
}

// Returns player bounding box
sf::FloatRect Player::getGlobalBounds() const {
    float halfWidth = size.x / 2.f;
    float halfHeight = size.y / 2.f;

    return sf::FloatRect(
        position.x - halfWidth,
        position.y - halfHeight,
        size.x,
        size.y
    );
}

// --- DIRECTION HANDLING ---
void Player::setDirection(Direction dir) {
    currentDirection = dir;
}

Direction Player::getDirection() const {
    return currentDirection;
}

void Player::setMoving(bool m) { moving = m; }

bool Player::isMoving() const { return moving; }

// Inventory access
Inventory &Player::getInventory() { return inventory; }

const Inventory& Player::getInventory() const {
    return inventory;
}

// --- ATTACK LOGIC ---
AttackInfo Player::tryAttack() {
    AttackInfo info;
    info.valid = false; // No attack by default

    // Attack must be requested explicitly
    if (!attackRequested) return info;

    attackRequested = false;

    // 1. Cooldown check
    if (attackCooldownTimer > 0.f) {
        return info;
    }

    // 2. Get selected item from inventory
    int slotIndex = inventory.getSelectedSlot();
    auto& slots = inventory.getSlots();

    // Check if slot is valid and contains an item
    if (slotIndex < 0 || slotIndex >= (int)slots.size() || !slots[slotIndex].has_value()) {
        return info;
    }

    const Item& item = slots[slotIndex].value();

    // Check if the item is a weapon
    if (item.type != ItemType::Weapon) {
        return info;
    }

    // 3. Valid attack setup
    info.valid = true;
    info.damage = item.damage;
    info.isProjectile = item.isProjectile;

    // Apply weapon cooldown
    attackCooldownTimer = item.cooldown;

    // 4. Compute direction vector based on player facing
    sf::Vector2f dirVec(0.f, 0.f);
    switch (currentDirection) {
        case Direction::Up:    dirVec = {0.f, -1.f}; break;
        case Direction::Down:  dirVec = {0.f, 1.f};  break;
        case Direction::Left:  dirVec = {-1.f, 0.f}; break;
        case Direction::Right: dirVec = {1.f, 0.f};  break;
    }

    // 5. Configure attack type
    if (info.isProjectile) {
        // --- PROJECTILE ATTACK ---
        info.startPosition = position;
        info.velocity = dirVec * item.projectileSpeed;
    }
    else {
        // --- MELEE ATTACK ---
        float reach = item.range;
        float width = 40.f;

        // Compute hitbox center in front of the player
        float offset = (std::max(size.x, size.y) / 2.f) + (reach / 2.f);
        sf::Vector2f hitCenter = position + (dirVec * offset);

        // Hitbox size depends on attack direction
        float boxW, boxH;
        if (currentDirection == Direction::Up || currentDirection == Direction::Down) {
            boxW = width;
            boxH = reach;
        } else {
            boxW = reach;
            boxH = width;
        }

        info.meleeHitbox = sf::FloatRect(
            hitCenter.x - boxW / 2.f,
            hitCenter.y - boxH / 2.f,
            boxW,
            boxH
        );
    }

    return info;
}

// Adds movement input (can be accumulated)
void Player::addMovement(const sf::Vector2f& dir) {
    movementIntent += dir;
}

// Returns and resets movement input
sf::Vector2f Player::consumeMovement() {
    sf::Vector2f result = movementIntent;
    movementIntent = {0.f, 0.f};
    return result;
}

// Requests an attack for the next update
void Player::requestAttack() {
    attackRequested = true;
}