#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>

#include "Player.h"

enum class ZombieType {
    Basic,
    Fast,
    Tank,
    Boss
};

class Enemy {
public:
    virtual ~Enemy() = default;

    virtual ZombieType getType() const = 0;
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
    float attackCooldown;
    sf::Clock attackClock;
    int attackDamage;
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ENEMY_H