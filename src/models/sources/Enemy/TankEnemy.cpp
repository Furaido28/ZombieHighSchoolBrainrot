#include "../../headers/Enemy/TankEnemy.h"

TankEnemy::TankEnemy(const sf::Vector2f& startPos)
    : BasicEnemy(startPos)
{
    speed = 40.f;
    radius = 50.f;
    health = 100;
}