#pragma once
#include "../Enemy.h"

class BasicEnemy : public Enemy {
public:
    explicit BasicEnemy(const sf::Vector2f& startPos);

    EnemyType getType() const override { return EnemyType::Basic; }

    void update(float dt, const sf::Vector2f& playerPos) override;

    int getDamage() const override;
    void attack(Player& player) override;
};
