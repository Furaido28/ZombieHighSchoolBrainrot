#ifndef ZOMBIEHIGHSCHOOLBRAINROT_BOSS_H
#define ZOMBIEHIGHSCHOOLBRAINROT_BOSS_H

#pragma once
#include "Enemy.h"

class Boss : public Enemy {
public:
    Boss(const sf::Vector2f& startPos);

    void update(float dt, const sf::Vector2f& playerPos) override;

    sf::Vector2f getPosition() const override;
    float getRadius() const override;

    int getDamage() const override;
    bool isAlive() const override;

private:
    sf::Vector2f position;
    float speed;
    float radius;
    int health;

    float attackCooldown;
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_BOSS_H