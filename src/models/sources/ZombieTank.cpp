#include "../headers/ZombieTank.h"

ZombieTank::ZombieTank(const sf::Vector2f& startPos)
    : ZombieBasic(startPos)
{
    speed = 40.f;
    radius = 26.f;
    health = 150;
}