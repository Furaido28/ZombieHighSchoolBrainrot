#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ENEMY_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ENEMY_H

#pragma once
#include <SFML/Graphics/Rect.hpp>

class Enemy {
public:
    virtual ~Enemy() = default;

    virtual void update(float dt, const sf::Vector2f& playerPos) = 0;

    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() const = 0;

    virtual float getRadius() const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;

    virtual int getDamage() const = 0;
    virtual bool isAlive() const = 0;

};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ENEMY_H