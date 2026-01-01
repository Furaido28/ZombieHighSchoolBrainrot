#include "models/headers/Enemy/BasicEnemy.h"
#include "models/headers/Player.h"
#include <cmath>

BasicEnemy::BasicEnemy(const sf::Vector2f& startPos)
    : Enemy(startPos),
      position(startPos),
      speed(60.f),
      radius(50.f),
      health(50.f) {}

void BasicEnemy::update(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f) dir /= len;

    velocity = dir * speed;
    position += velocity * dt;
}

void BasicEnemy::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f BasicEnemy::getPosition() const {
    return position;
}

sf::Vector2f BasicEnemy::getVelocity() const {
    return velocity;
}

float BasicEnemy::getRadius() const {
    return radius;
}

sf::FloatRect BasicEnemy::getGlobalBounds() const {
    return {
        position.x - radius,
        position.y - radius,
        radius * 2.f,
        radius * 2.f
    };
}

float BasicEnemy::getHealth() const {
    return health;
}

float BasicEnemy::getMaxHealth() const {
    return 50.f;
}

bool BasicEnemy::isAlive() const {
    return health > 0.f;
}

int BasicEnemy::getDamage() const {
    return 10;
}

void BasicEnemy::attack(Player& player) {
    if (!canAttack()) return;
    player.takeDamage(getDamage());
    attackClock.restart();
}
