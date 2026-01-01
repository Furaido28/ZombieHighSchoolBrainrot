#include "models/headers/Boss/TralaleroTralala.h"

#include <iostream>

#include "models/headers/Player.h"

TralaleroTralala::TralaleroTralala(const sf::Vector2f& pos)
    : Boss(pos)
{
    maxHealth = 600.f;
    health = maxHealth;
    attackCooldown = 1.5f;
}

void TralaleroTralala::updatePhaseLogic(float dt,
                                       const sf::Vector2f& playerPos)
{
    switch (currentPhase) {

        // Phase 1 : lent, basique
        case 0:
            speed = 50.f;
            moveTowardPlayer(dt, playerPos);
            break;

            // Phase 2 : dash occasionnel
        case 1:
            speed = 80.f;
            dashTowardPlayer(playerPos);
            moveTowardPlayer(dt, playerPos);
            break;

            // Phase 3 : dash + shockwave
        case 2:
            speed = 120.f;
            dashTowardPlayer(playerPos);
            shockwave();
            moveTowardPlayer(dt, playerPos);
            break;

            // Phase 4 : berserk
        case 3:
            speed = 160.f;
            dashTowardPlayer(playerPos);
            shockwave();
            break;
    }
}


int TralaleroTralala::getDamage() const {
    return 25 + currentPhase * 10;
}

void TralaleroTralala::attack(Player& player) {
    if (!player.isAlive()) return;
    if (!canAttack()) return;

    player.takeDamage(getDamage());
    attackClock.restart();
}

void TralaleroTralala::dashTowardPlayer(const sf::Vector2f& playerPos)
{
    if (dashClock.getElapsedTime().asSeconds() < 3.f)
        return;

    sf::Vector2f dir = normalize(playerPos - position);
    velocity = dir * 400.f; // dash violent

    dashClock.restart();
}

void TralaleroTralala::shockwave()
{
    if (shockwaveClock.getElapsedTime().asSeconds() < 5.f)
        return;

    // logique AOE (collision cercle autour du boss)
    std::cout << "SHOCKWAVE!\n";

    shockwaveClock.restart();
}

