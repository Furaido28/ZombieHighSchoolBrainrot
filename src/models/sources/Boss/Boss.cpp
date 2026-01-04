#include "models/headers/Boss/Boss.h"
#include <cmath>
#include <iostream>

#include "models/headers/Item.h"

// Constructor: initializes boss properties
Boss::Boss(const sf::Vector2f& startPos) : Enemy(startPos) {
    speed = 60.f;
    renderRadius = 100.f;
    collisionRadius = 48.f;
}

// Updates boss behavior every frame
void Boss::update(float dt, const sf::Vector2f& playerPos)
{
    // Update boss phase based on current health
    updatePhase();

    // Update current phase behavior
    if (currentPhase)
        currentPhase->update(*this, dt, playerPos);
}

// Determines the boss phase according to health ratio
void Boss::updatePhase()
{
    float ratio = health / maxHealth;
    int newPhase = 0;

    if (ratio > 0.75f) newPhase = 0;
    else if (ratio > 0.5f) newPhase = 1;
    else if (ratio > 0.25f) newPhase = 2;
    else newPhase = 3;

    // Change phase only if needed
    if (newPhase != currentPhaseIndex) {
        currentPhaseIndex = newPhase;
        setPhase(createPhase(newPhase));
    }
}

// Switches to a new boss phase
void Boss::setPhase(std::unique_ptr<BossPhase> phase)
{
    // Clean exit from previous phase
    if (currentPhase)
        currentPhase->onExit(*this);

    currentPhase = std::move(phase);

    // Initialize new phase
    if (currentPhase)
        currentPhase->onEnter(*this);
}

// Moves the boss toward the player position
void Boss::moveTowardPlayer(float dt, const sf::Vector2f& playerPos) {
    sf::Vector2f dir = playerPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    // Normalize direction and apply movement
    if (len > 0.f) {
        dir /= len;
        velocity = dir * speed;
        position += velocity * dt;
    }
}

// Called when the boss dies
void Boss::onDeath() {
    // Prevent multiple drops
    if (keyFragmentDropped)
        return;

    keyFragmentDropped = true;
    std::cout << "BOSS DEFEATED" << std::endl;
}