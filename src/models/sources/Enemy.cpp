#include "models/headers/Enemy.h"

#include <cmath>

Enemy::Enemy(const sf::Vector2f& startPos){
    setPosition(startPos);
    velocity = {0.f, 0.f};
    speed = 100.f;
    maxHealth = 50.f;
    health = maxHealth;
    renderRadius = 50.f;
    collisionRadius = 24.f;
    radius = renderRadius;
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
    return renderRadius;
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
        position.x - collisionRadius,
        position.y - collisionRadius,
        collisionRadius * 2.f,
        collisionRadius * 2.f
    };
}

bool Enemy::canAttack() const {
    return attackClock.getElapsedTime().asSeconds() >= attackCooldown;
}

sf::Vector2f Enemy::normalize(const sf::Vector2f& v) const {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len == 0.f)
        return {0.f, 0.f};
    return { v.x / len, v.y / len };
}

void Enemy::takeDamage(int amount) {
    if (!isAlive()) {
        return;
    }
    health -= amount;
    if (health <0)
        health = 0;
}

void Enemy::applyArchetype(const EnemyArchetype &data) {
    speed = data.speed;
    maxHealth = data.maxHealth;
    health = maxHealth;
    attackCooldown = data.attackCooldown;
    renderRadius = data.renderRadius;
    collisionRadius = data.collisionRadius;
    radius = renderRadius;
}
