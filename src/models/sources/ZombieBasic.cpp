#include "../headers/ZombieBasic.h"
#include <cmath>
#include <SFML/Graphics/Rect.hpp>

ZombieBasic::ZombieBasic(const sf::Vector2f& startPos)
    : position(startPos),
      speed(60.f),
      radius(18.f),
      health(50)
{}

void ZombieBasic::update(float dt, const sf::Vector2f& playerPos)
{
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
        dir /= len;

    position += dir * speed * dt;
}

void ZombieBasic::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f ZombieBasic::getPosition() const { return position; }

float ZombieBasic::getRadius() const { return radius; }

sf::FloatRect ZombieBasic::getGlobalBounds() const {
    return {
        position.x - radius,
        position.y - radius,
        radius * 2.f,
        radius * 2.f
    };
}

int ZombieBasic::getDamage() const { return 10; }

bool ZombieBasic::isAlive() const { return health > 0; }