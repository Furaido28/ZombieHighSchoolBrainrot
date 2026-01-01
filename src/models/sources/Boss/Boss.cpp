#include "models/headers/Boss/Boss.h"
#include <cmath>

Boss::Boss(const sf::Vector2f& startPos)
    : Enemy(startPos)
{
    speed = 60.f;
    radius = 40.f;
    health = 800;
    velocity = {0.f, 0.f};
}

void Boss::update(float dt, const sf::Vector2f& playerPos)
{
    // IA de base : suivre le joueur
    sf::Vector2f dir = playerPos - position;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.f)
        dir /= len;

    velocity = dir * speed;
    position += velocity * dt;
}
