#include "../headers/Boss.h"

#include <cmath>

Boss::Boss(const sf::Vector2f& startPos)
    : position(startPos),
      speed(50.f),
      radius(50.f),
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

    position += dir * speed * dt;
}

sf::Vector2f Boss::getPosition() const { return position; }
float Boss::getRadius() const { return radius; }
int Boss::getDamage() const { return 40; }
bool Boss::isAlive() const { return health > 0; }
