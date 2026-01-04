#pragma once
#include "models/headers/Enemy.h"
#include "models/headers/Enemy/EnemyArchetype.h"

// Generic enemy class using shared archetype data
class GenericEnemy : public Enemy {
public:
    // Constructor: creates an enemy using a shared archetype
    GenericEnemy(const sf::Vector2f& startPos, const EnemyArchetype& archetype);

    // Returns the enemy type from its archetype
    EnemyType getType() const override {
        return data.type;
    }

    // Updates enemy behavior (movement toward player)
    void update(float dt, const sf::Vector2f& playerPos) override;

    // Returns damage value from the archetype
    int getDamage() const override {
        return data.damage;
    }

private:
    // Reference to shared archetype data (Flyweight pattern)
    const EnemyArchetype& data;
};
