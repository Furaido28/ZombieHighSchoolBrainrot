#include "../headers/Player.h"

// 1. MODIFICATION : On initialise 'currentDirection' à Down (Face) par défaut
Player::Player()
    : position(0.f, 0.f), size(48.f, 48.f), currentDirection(Direction::Down), moving(false)
{
    maxHealth = 100;
    health = maxHealth;
}

void Player::takeDamage(int amount){
    if (invincibilityTimer > 0.f)
        return;

    health -= amount;
    if (health < 0)
        health = 0;

    invincibilityTimer = 3.f; // 3s d’invincibilité
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}
void Player::setHealth(int value) {
    health = value;
}


bool Player::isInvincible() const{
    return invincibilityTimer > 0.f;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::move(const sf::Vector2f& delta) {
    position += delta;
}

void Player::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
}

void Player::setSize(float w, float h) {
    size = sf::Vector2f(w, h);
}

void Player::update(float dt) {
    if (invincibilityTimer > 0.f)
        invincibilityTimer -= dt;
}

sf::Vector2f Player::getPosition() const {
    return position;
}

sf::Vector2f Player::getSize() const {
    return size;
}

float Player::getRadius() const {
    return std::min(size.x, size.y) * 0.5f;
}

// Calcule et retourne la Hitbox du joueur (Ton code était parfait ici)
sf::FloatRect Player::getGlobalBounds() const {
    float halfWidth = size.x / 2.f;
    float halfHeight = size.y / 2.f;

    // On retourne le coin supérieur gauche du rectangle
    return sf::FloatRect(
        position.x - halfWidth,
        position.y - halfHeight,
        size.x,
        size.y
    );
}

// 2. AJOUT : Implémentation des méthodes de direction
void Player::setDirection(Direction dir) {
    currentDirection = dir;
}

Direction Player::getDirection() const {
    return currentDirection;
}

void Player::setMoving(bool m) { moving = m; }
bool Player::isMoving() const { return moving; }
Inventory &Player::getInventory() {return inventory;}
