#include "models/headers/Enemy/FastEnemy.h"

FastEnemy::FastEnemy(const sf::Vector2f& startPos) : BasicEnemy(startPos) {
    speed = 150.f;
    maxHealth = 50.f;
    health = maxHealth;
    attackCooldown = 1.5f;
}

int FastEnemy::getDamage() const {
    return 10;
}
