#pragma once
#include <functional>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

#include "Enemy/EnemyArchetype.h"
#include "Enemy/EnemyType.h"

class Player;

// Base class for all enemies
class Enemy {
public:
    // Constructor: initializes enemy at a given position
    explicit Enemy(const sf::Vector2f& startPos);
    virtual ~Enemy() = default;

    // Must be implemented by derived classes
    virtual EnemyType getType() const = 0;
    virtual void update(float dt, const sf::Vector2f& playerPos) = 0;

    // --- Position & movement ---
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;

    // --- Health ---
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;

    // --- Combat ---
    // Damage value depends on the enemy type
    virtual int getDamage() const = 0;

    // Attacks the player if possible
    void attack(Player& player);

    // --- Collision ---
    float getRadius() const;
    virtual sf::FloatRect getGlobalBounds() const;

    // Checks if attack cooldown is finished
    bool canAttack() const;

    // Applies damage to the enemy
    virtual void takeDamage(int amount);

    // Applies shared archetype values (Flyweight pattern)
    void applyArchetype(const EnemyArchetype& data);

    // Sets a callback triggered when the enemy dies
    void setDeathCallback(std::function<void(const Enemy&)> cb) {
        deathCallback = std::move(cb);
    }

protected:
    // Position and movement data
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;

    // Health values
    float health;
    float maxHealth;

    // Rendering and collision sizes
    float radius;
    float renderRadius;
    float collisionRadius;

    // --- Attack handling ---
    sf::Clock attackClock;
    float attackCooldown;

    // Utility function to normalize a vector
    sf::Vector2f normalize(const sf::Vector2f& v) const;

    // Called once when the enemy dies
    virtual void onDeath(){}

    // Optional callback for external systems (score, loot, etc.)
    std::function<void(const Enemy&)> deathCallback;

    // Prevents death logic from running multiple times
    bool deadHandled = false;
};
