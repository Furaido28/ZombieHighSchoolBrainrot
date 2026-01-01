#include "models/headers/Boss/OscarTheCrackhead.h"

OscarTheCrackhead::OscarTheCrackhead(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 600.f;
    health = maxHealth;
    attackCooldown = 1.5f;
}

void OscarTheCrackhead::updatePhaseLogic(float dt, const sf::Vector2f& playerPos) {
    switch (currentPhase) {
        case 0:
            speed = 50.f;
            break;
        case 1:
            speed = 80.f;
            break;
        case 2:
            speed = 120.f;
            break;
        case 3:
            speed = 160.f;
            break;
    }

    moveTowardPlayer(dt, playerPos);
}

int OscarTheCrackhead::getDamage() const {
    return 25 + currentPhase * 10;
}

void OscarTheCrackhead::attack(Player& player) {
    if (!player.isAlive()) return;
    if (!canAttack()) return;

    player.takeDamage(getDamage());
    attackClock.restart();
}