#pragma once
#include "EnemyType.h"

// Shared data structure for enemy statistics (Flyweight pattern)
struct EnemyArchetype {
    // Type of enemy this archetype represents
    EnemyType type;

    // Movement speed of the enemy
    float speed;

    // Maximum health value
    float maxHealth;

    // Damage dealt per attack
    int damage;

    // Time between two attacks
    float attackCooldown;

    // Radius used for rendering
    float renderRadius;

    // Radius used for collision detection
    float collisionRadius;
};
