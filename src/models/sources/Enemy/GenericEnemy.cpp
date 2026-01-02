#include "models/headers/Enemy/GenericEnemy.h"
#include "models/headers/Player.h"
#include <cmath>

GenericEnemy::GenericEnemy(const sf::Vector2f& startPos, const EnemyArchetype& archetype)
    : Enemy(startPos), data(archetype)
{
    applyArchetype(data);
}

void GenericEnemy::update(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 0.f) {
        dir /= length;
        velocity = dir * speed;
        position += velocity * dt;
    }
}

void GenericEnemy::attack(Player& player) {
    if (!isAlive()) return;
    if (!canAttack()) return;

    player.takeDamage(getDamage());
    attackClock.restart();
}
