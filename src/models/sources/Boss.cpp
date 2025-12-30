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
    attackCooldown -= dt;

    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
        dir /= len;

    velocity = dir * speed * dt;
    position += velocity;
}

void Boss::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f Boss::getVelocity() const {
    return velocity;
}


sf::Vector2f Boss::getPosition() const { return position; }

float Boss::getRadius() const { return radius; }

sf::FloatRect Boss::getGlobalBounds() const {
    return {
        position.x - radius,
        position.y - radius,
        radius * 2.f,
        radius * 2.f
    };
}

int Boss::getDamage() const { return 40; }

bool Boss::isAlive() const { return health > 0; }

