#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

class Player;

enum class EnemyType {
    Basic,
    Fast,
    Tank,
    Boss01,
    Boss02,
    Boss03,
    FinalBoss
};

class Enemy {
public:
    explicit Enemy(const sf::Vector2f& pos);
    virtual ~Enemy() = default;

    virtual EnemyType getType() const = 0;
    virtual void update(float dt, const sf::Vector2f& playerPos) = 0;

    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual sf::Vector2f getVelocity() const = 0;

    virtual float getRadius() const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;

    virtual float getHealth() const = 0;
    virtual float getMaxHealth() const = 0;
    virtual bool isAlive() const = 0;

    virtual int getDamage() const = 0;
    virtual void attack(Player& player) = 0;

    bool canAttack() const;

protected:
    sf::Clock attackClock;
    float attackCooldown = 1.f;
};
