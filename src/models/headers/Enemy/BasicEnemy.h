#pragma once
#include "models/headers/Enemy.h"

class BasicEnemy : public Enemy {
public:
    explicit BasicEnemy(const sf::Vector2f& startPos);

    EnemyType getType() const override { return EnemyType::Basic; }

    void update(float dt, const sf::Vector2f& playerPos) override;

    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;
    sf::Vector2f getVelocity() const override;

    float getRadius() const override;
    sf::FloatRect getGlobalBounds() const override;

    float getHealth() const override;
    float getMaxHealth() const override;
    bool isAlive() const override;

    int getDamage() const override;
    void attack(Player& player) override;

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float radius;
    float health;
};
