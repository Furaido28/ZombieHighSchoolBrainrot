#include "models/headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f&) {}

bool Enemy::canAttack() const {
    return attackClock.getElapsedTime().asSeconds() >= attackCooldown;
}
