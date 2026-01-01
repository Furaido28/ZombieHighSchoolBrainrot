#include "models/headers/Boss/OscarTheCrackhead.h"

OscarTheCrackhead::OscarTheCrackhead(const sf::Vector2f& startPos)
    : Boss(startPos)
{
    speed = 90.f;
    radius = 50.f;
    health = 1200;
}

void OscarTheCrackhead::update(float dt, const sf::Vector2f& playerPos)
{
    // Comportement commun
    Boss::update(dt, playerPos);

    // 🎯 Comportement spécifique (optionnel)
    // ex: dash, rage mode, etc.
}
