#include "../headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f& pos) {
    attackCooldown = 1.5f;
    attackDamage = 10;
    attackClock.restart();
}

bool Enemy::canAttack() const {
    return attackClock.getElapsedTime().asSeconds() >= attackCooldown;
}