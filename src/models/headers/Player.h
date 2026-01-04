#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Inventory.h"
#include "Item.h" // Needed to know weapon types

// Defines possible player directions
enum class Direction { Down = 0, Left = 1, Right = 2, Up = 3 };

// Structure used to return attack data to the controller
struct AttackInfo {
    // Indicates if the attack is valid (no cooldown, weapon equipped, etc.)
    bool valid = false;

    // true = projectile attack, false = melee attack
    bool isProjectile = false;

    // Damage dealt by the attack
    int damage = 0;

    // Melee attack hitbox (used if isProjectile == false)
    sf::FloatRect meleeHitbox;

    // Projectile attack data (used if isProjectile == true)
    sf::Vector2f startPosition;
    sf::Vector2f velocity;
};

// Player entity class
class Player {
public:
    Player();

    // --- Health ---
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int value);
    void takeDamage(int amount);
    bool isAlive() const;
    bool isInvincible() const;

    // --- Movement & Position ---
    void move(const sf::Vector2f& delta);
    void setPosition(float x, float y);
    void setSize(float w, float h);

    // Updates timers (invincibility and attack cooldown)
    void update(float dt);

    // Returns collision radius
    float getRadius() const;

    // --- Direction handling ---
    void setDirection(Direction dir);
    Direction getDirection() const;

    // --- Hitbox & Size ---
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getGlobalBounds() const;

    // --- State ---
    void setMoving(bool moving);
    bool isMoving() const;

    // Inventory access
    Inventory& getInventory();
    const Inventory& getInventory() const;

    // Tries to perform an attack and returns attack data
    AttackInfo tryAttack();

    // Adds movement input (can be accumulated)
    void addMovement(const sf::Vector2f& dir);

    // Returns and clears movement input
    sf::Vector2f consumeMovement();

    // Requests an attack (checked during update)
    void requestAttack();

private:
    // Health values
    int health;
    int maxHealth;

    // Invincibility timer after taking damage
    float invincibilityTimer = 0.f;

    // Attack cooldown timer to prevent attack spamming
    float attackCooldownTimer = 0.f;

    // Transform data
    sf::Vector2f position;
    sf::Vector2f size;
    float speed;

    // Movement state
    bool moving;

    // Player inventory
    Inventory inventory;

    // Current facing direction (used for attacks)
    Direction currentDirection;

    // Accumulated movement input
    sf::Vector2f movementIntent{0.f, 0.f};

    // Indicates that an attack was requested
    bool attackRequested = false;
};