#pragma once
#include "models/headers/Enemy.h"

struct EnemyArchetype {
    EnemyType type;

    float speed;
    float maxHealth;

    int damage;
    float attackCooldown;

    float renderRadius;
    float collisionRadius;
};
