#include "../headers/Boss.h"

#include <cmath>

Boss::Boss(const sf::Vector2f& startPos)
    : position(startPos),
      speed(50.f),
      radius(75.f),
      health(1000),
      attackCooldown(0.f)
{}

void Boss::update(float dt, const sf::Vector2f& playerPos)
{
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
        dir /= len;

    velocity = dir * speed;
    position += velocity * dt;
}

void Boss::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f Boss::getVelocity() const {
    return velocity;
}


sf::Vector2f Boss::getPosition() const { return position; }

sf::FloatRect Boss::getGlobalBounds() const {
    // Rayon de collision FIXE (indépendant du visuel)
    const float collisionRadius = 60.f;

    return {
        position.x - collisionRadius,
        position.y - collisionRadius,
        collisionRadius * 2.f,
        collisionRadius * 2.f
    };
}

int Boss::getDamage() const { return 40; }

bool Boss::isAlive() const { return health > 0; }

