#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

#include "Enemy/EnemyArchetype.h"
#include "Enemy/EnemyType.h"

class Player;


class Enemy {
public:
    explicit Enemy(const sf::Vector2f& startPos);
    virtual ~Enemy() = default;

    virtual EnemyType getType() const = 0;
    virtual void update(float dt, const sf::Vector2f& playerPos) = 0;

    // Position & mouvement
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;

    // Vie
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;

    // Combat
    virtual int getDamage() const = 0;
    virtual void attack(Player& player) = 0;

    // Collision
    float getRadius() const;
    virtual sf::FloatRect getGlobalBounds() const;

    bool canAttack() const;
    virtual void takeDamage(int amount);
    void applyArchetype(const EnemyArchetype& data);

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;

    float health;
    float maxHealth;

    float radius;
    float renderRadius;
    float collisionRadius;

    // Attaque
    sf::Clock attackClock;
    float attackCooldown;

    sf::Vector2f normalize(const sf::Vector2f& v) const;
};
