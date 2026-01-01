#include "models/headers/Enemy/BasicEnemy.h"
#include <cmath>

#include "models/headers/Player.h"

BasicEnemy::BasicEnemy(const sf::Vector2f& startPos)
    : Enemy(startPos) {
    speed = 100.f;
    maxHealth = 50.f;
    health = maxHealth;
    attackCooldown = 1.f;
}

void BasicEnemy::update(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 0.f) {
        dir /= length;
        velocity = dir * speed;
        position += velocity * dt;
    }
}

int BasicEnemy::getDamage() const {
    return 10;
}

void BasicEnemy::attack(Player& player) {
    if (!isAlive()) return;
    if (!canAttack()) return;

    player.takeDamage(getDamage());
    attackClock.restart();
}
