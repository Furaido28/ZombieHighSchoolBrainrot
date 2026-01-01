#include "../../headers/Enemy/FastEnemy.h"

FastEnemy::FastEnemy(const sf::Vector2f& startPos)
    : BasicEnemy(startPos)
{
    speed = 100.f;
    radius = 35.0f;
    health = 35;
}
