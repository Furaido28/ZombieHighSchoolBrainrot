#include "../../headers/Enemy/BasicEnemy"
#include <cmath>
#include <SFML/Graphics/Rect.hpp>

#include "models/headers/Player.h"

BasicEnemy::BasicEnemy(const sf::Vector2f& startPos)
    : Enemy(startPos),
      speed(60.f),
      radius(50.f),
      health(50) {
}

void BasicEnemy::update(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
        dir /= len;

    velocity = dir * speed;
    position += velocity * dt;
}


void BasicEnemy::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f BasicEnemy::getPosition() const { return position; }

sf::Vector2f BasicEnemy::getVelocity() const {
    return velocity;
}

sf::FloatRect BasicEnemy::getGlobalBounds() const {
    // Rayon de collision FIXE (indépendant du visuel)
    const float collisionRadius = 32.f;

    return {
        position.x - collisionRadius,
        position.y - collisionRadius,
        collisionRadius * 2.f,
        collisionRadius * 2.f
    };
}

void BasicEnemy::attack(Player& player) {
    if (!player.isAlive()) return;
    if (!canAttack()) return;

    player.takeDamage(getDamage());

    attackClock.restart();
}

bool BasicEnemy::isAlive() const { return health > 0; }