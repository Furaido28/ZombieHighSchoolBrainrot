#include "../headers/ZombieBasic.h"
#include <cmath>
#include <SFML/Graphics/Rect.hpp>

ZombieBasic::ZombieBasic(const sf::Vector2f& startPos)
    : position(startPos),
    speed(60.f),
    radius(50.f),
    health(50)
{}

void ZombieBasic::update(float dt, const sf::Vector2f& playerPos)
{
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
        dir /= len;

    velocity = dir * speed;
    position += velocity * dt;
}


void ZombieBasic::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f ZombieBasic::getPosition() const { return position; }

sf::Vector2f ZombieBasic::getVelocity() const {
    return velocity;
}

sf::FloatRect ZombieBasic::getGlobalBounds() const
{
    // Rayon de collision FIXE (indépendant du visuel)
    const float collisionRadius = 32.f;

    return {
        position.x - collisionRadius,
        position.y - collisionRadius,
        collisionRadius * 2.f,
        collisionRadius * 2.f
    };
}


int ZombieBasic::getDamage() const { return 10; }

bool ZombieBasic::isAlive() const { return health > 0; }