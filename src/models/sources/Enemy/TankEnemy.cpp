#include "models/headers/Enemy/TankEnemy.h"

TankEnemy::TankEnemy(const sf::Vector2f& startPos) : BasicEnemy(startPos) {
    speed = 75.f;
    radius = 60.f;
    maxHealth = 100.f;
    health = maxHealth;
    attackCooldown = 2.5f;
}

int TankEnemy::getDamage() const {
    return 20;
}
