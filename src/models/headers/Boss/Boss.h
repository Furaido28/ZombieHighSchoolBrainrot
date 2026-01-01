#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "models/headers/Enemy.h"

class Boss : public Enemy {
public:
    explicit Boss(const sf::Vector2f& startPos);

    // logique commune
    void update(float dt, const sf::Vector2f& playerPos) override;

    virtual int getDamage() const override = 0;

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float radius;
    int health;

    float attackCooldown = 1.5f;
};
