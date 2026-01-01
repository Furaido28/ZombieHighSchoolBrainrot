#include "models/headers/Boss/Boss.h"
#include <cmath>

Boss::Boss(const sf::Vector2f& startPos) : Enemy(startPos) {
    speed = 60.f;
    radius = 100.f;
}

void Boss::update(float dt, const sf::Vector2f& playerPos) {
    phaseTimer += dt;

    updatePhase();
    updatePhaseLogic(dt, playerPos);
}

void Boss::updatePhase() {
    float ratio = health / maxHealth;

    if (ratio < 0.7f) currentPhase = 1;
    if (ratio < 0.4f) currentPhase = 2;
    if (ratio < 0.2f) currentPhase = 3;
}

void Boss::moveTowardPlayer(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f) {
        dir /= len;
        velocity = dir * speed;
        position += velocity * dt;
    }
}
