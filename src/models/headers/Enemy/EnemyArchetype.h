#pragma once
#include "EnemyType.h"

struct EnemyArchetype {
    EnemyType type;

    float speed;
    float maxHealth;

    int damage;
    float attackCooldown;

    float renderRadius;
    float collisionRadius;
};
