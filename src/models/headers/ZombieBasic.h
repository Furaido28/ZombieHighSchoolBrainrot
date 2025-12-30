#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEBASIC_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEBASIC_H

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


    float getRadius() const override;
    sf::FloatRect getGlobalBounds() const override;

    float getHealth() const override { return health; }
    float getMaxHealth() const override { return 50.f; }
    int getDamage() const override;
    bool isAlive() const override;

    ZombieType getType() const override {
        return ZombieType::Basic;
    }


protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float radius;
    int health;
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ZOMBIEBASIC_H