#include "../headers/ZombieFast.h"

ZombieFast::ZombieFast(const sf::Vector2f& startPos)
    : ZombieBasic(startPos)
{
    speed = 120.f;
    radius = 50.0f;
    health = 35;
}
