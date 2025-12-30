#include "../headers/ZombieFast.h"

ZombieFast::ZombieFast(const sf::Vector2f& startPos)
    : ZombieBasic(startPos)
{
    speed = 100.f;
    radius = 35.0f;
    health = 35;
}
