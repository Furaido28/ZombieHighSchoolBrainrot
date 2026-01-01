#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Enemy.h"

class ZombieBasic : public Enemy {
public:
    ZombieBasic(const sf::Vector2f& startPos);

    void update(float dt, const sf::Vector2f& playerPos) override;

    void setPosition(const sf::Vector2f &pos) override;
    sf::Vector2f getPosition() const override;
    sf::Vector2f getVelocity() const override;

    ZombieType getType() const override { return ZombieType::Basic; }

    float getRadius() const override { return radius; }
    sf::FloatRect getGlobalBounds() const override;

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 50.f; }
    bool isAlive() const override;

    int getDamage() const override { return attackDamage; }
    void attack(Player& player) override;

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float radius;
    int health;

    float attackCooldown = 1.5;
    int attackDamage = 15;
};