#include "models/headers/Enemy.h"

#include <cmath>
#include <iostream>

#include "models/headers/Player.h"

// Constructor: initializes default enemy values
Enemy::Enemy(const sf::Vector2f& startPos){
    // Set initial position
    setPosition(startPos);

    // Initial movement values
    velocity = {0.f, 0.f};
    speed = 100.f;

    // Health values
    maxHealth = 50.f;
    health = maxHealth;

    // Rendering and collision sizes
    renderRadius = 50.f;
    collisionRadius = 24.f;
    radius = renderRadius;

    // Time between two attacks
    attackCooldown = 1.f;
}

// Sets the enemy position
void Enemy::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

// Returns the enemy position
sf::Vector2f Enemy::getPosition() const {
    return position;
}

// Returns the current velocity
sf::Vector2f Enemy::getVelocity() const {
    return velocity;
}

// Returns the render radius (used for drawing)
float Enemy::getRadius() const {
    return renderRadius;
}

// Returns current health
float Enemy::getHealth() const {
    return health;
}

// Returns maximum health
float Enemy::getMaxHealth() const {
    return maxHealth;
}

// Checks if the enemy is still alive
bool Enemy::isAlive() const {
    return health > 0.f;
}

// Returns the collision bounding box
sf::FloatRect Enemy::getGlobalBounds() const {
    return {
        position.x - collisionRadius,
        position.y - collisionRadius,
        collisionRadius * 2.f,
        collisionRadius * 2.f
    };
}

// Checks if the enemy can attack (cooldown elapsed)
bool Enemy::canAttack() const {
    return attackClock.getElapsedTime().asSeconds() >= attackCooldown;
}

// Normalizes a vector (keeps direction, length = 1)
sf::Vector2f Enemy::normalize(const sf::Vector2f& v) const {
    float len = std::sqrt(v.x * v.x + v.y * v.y);

    // Avoid division by zero
    if (len == 0.f)
        return {0.f, 0.f};

    return { v.x / len, v.y / len };
}

// Applies damage to the enemy
void Enemy::takeDamage(int amount) {
    // Do nothing if already dead
    if (!isAlive()) {
        return;
    }

    // Reduce health
    health -= amount;

    // Handle death only once
    if (health <= 0 && !deadHandled) {
        deadHandled = true;
        health == 0;

        // Trigger death logic
        onDeath();

        // Notify listeners if a callback exists
        if (deathCallback) {
            std::cout << "[DEBUG] Enemy deathCallback triggered\n";
            deathCallback(*this);
        }
    }
}

// Enemy attacks the player
void Enemy::attack(Player &player) {
    // Cannot attack if dead or on cooldown
    if (!isAlive()) return;
    if (!canAttack()) return;

    // Store player health to detect a hit
    int healthValue = player.getHealth();

    // Random chance to hit the player
    if (std::rand() % 100 < 45)
        player.takeDamage(getDamage());

    // Reset attack cooldown
    attackClock.restart();

    // Debug message if the attack was successful
    if (healthValue != player.getHealth()) {
        std::cout << "HIT !" << std::endl;
    }
}

// Applies shared archetype values to this enemy
void Enemy::applyArchetype(const EnemyArchetype &data) {
    speed = data.speed;
    maxHealth = data.maxHealth;
    health = maxHealth;
    attackCooldown = data.attackCooldown;
    renderRadius = data.renderRadius;
    collisionRadius = data.collisionRadius;
    radius = renderRadius;
}