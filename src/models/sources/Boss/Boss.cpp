#include "models/headers/Boss/Boss.h"
#include <cmath>
#include <iostream>

#include "models/headers/Item.h"

Boss::Boss(const sf::Vector2f& startPos) : Enemy(startPos) {
    speed = 60.f;
    renderRadius = 100.f;
    collisionRadius = 48.f;
}

void Boss::update(float dt, const sf::Vector2f& playerPos)
{
    updatePhase();

    if (currentPhase)
        currentPhase->update(*this, dt, playerPos);
}


void Boss::updatePhase()
{
    float ratio = health / maxHealth;
    int newPhase = 0;

    if (ratio > 0.75f) newPhase = 0;
    else if (ratio > 0.5f) newPhase = 1;
    else if (ratio > 0.25f) newPhase = 2;
    else newPhase = 3;

    if (newPhase != currentPhaseIndex) {
        currentPhaseIndex = newPhase;
        setPhase(createPhase(newPhase));
    }
}

void Boss::setPhase(std::unique_ptr<BossPhase> phase)
{
    if (currentPhase)
        currentPhase->onExit(*this);

    currentPhase = std::move(phase);

    if (currentPhase)
        currentPhase->onEnter(*this);
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
void Boss::onDeath() {
    if (keyFragmentDropped)
        return;
    keyFragmentDropped = true;
    std::cout << "BOSS DEFEATED" << std::endl;

    //TODO : notifier le GameController
}

