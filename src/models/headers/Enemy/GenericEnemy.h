#pragma once
#include "models/headers/Enemy.h"
#include "models/headers/Enemy/EnemyArchetype.h"

class GenericEnemy : public Enemy {
public:
    GenericEnemy(const sf::Vector2f& startPos, const EnemyArchetype& archetype);

    EnemyType getType() const override { return data.type; }
    void update(float dt, const sf::Vector2f& playerPos) override;

    int getDamage() const override { return data.damage; }

private:
    const EnemyArchetype& data;
};
