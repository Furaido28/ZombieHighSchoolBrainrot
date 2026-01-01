#include "models/headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f& startPos){
    setPosition(startPos);
    velocity = {0.f, 0.f};
    speed = 100.f;
    maxHealth = 50.f;
    health = maxHealth;
    radius = 50.f;
    attackCooldown = 1.f;
}

void Enemy::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

sf::Vector2f Enemy::getVelocity() const {
    return velocity;
}

float Enemy::getRadius() const {
    return radius;
}

float Enemy::getHealth() const {
    return health;
}

float Enemy::getMaxHealth() const {
    return maxHealth;
}

bool Enemy::isAlive() const {
    return health > 0.f;
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return {
        position.x - radius,
        position.y - radius,
        radius * 2.f,
        radius * 2.f
    };
}

bool Enemy::canAttack() const {
    return attackClock.getElapsedTime().asSeconds() >= attackCooldown;
}
