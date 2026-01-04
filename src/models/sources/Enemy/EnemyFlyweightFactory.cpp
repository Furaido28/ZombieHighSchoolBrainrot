#include "models/headers/Enemy/EnemyFlyweightFactory.h"

// Returns a shared archetype for a given enemy type
// Uses a cache to avoid creating the same archetype multiple times
const EnemyArchetype& EnemyFlyweightFactory::get(EnemyType type) {
    // Check if the archetype already exists in the cache
    auto it = cache.find(type);
    if (it != cache.end())
        return *it->second;

    // Create a new archetype if it does not exist
    auto a = std::make_unique<EnemyArchetype>();
    a->type = type;

    // Default values depending on the enemy type
    switch (type) {
        case EnemyType::Basic:
            // Standard enemy with balanced stats
            a->speed = 100.f;
            a->maxHealth = 50.f;
            a->damage = 10;
            a->attackCooldown = 1.f;
            a->renderRadius = 50.f;
            a->collisionRadius = 24.f;
            break;

        case EnemyType::Fast:
            // Fast enemy with lower health and damage
            a->speed = 160.f;
            a->maxHealth = 35.f;
            a->damage = 8;
            a->attackCooldown = 0.8f;
            a->renderRadius = 48.f;
            a->collisionRadius = 22.f;
            break;

        case EnemyType::Tank:
            // Slow but strong enemy with high health and damage
            a->speed = 70.f;
            a->maxHealth = 120.f;
            a->damage = 18;
            a->attackCooldown = 1.2f;
            a->renderRadius = 60.f;
            a->collisionRadius = 28.f;
            break;

        default:
            // Fallback values (bosses are handled elsewhere)
            a->speed = 100.f;
            a->maxHealth = 50.f;
            a->damage = 10;
            a->attackCooldown = 1.f;
            a->renderRadius = 50.f;
            a->collisionRadius = 24.f;
            break;
    }

    // Store the archetype in the cache and return a reference to it
    auto& ref = *a;
    cache.emplace(type, std::move(a));
    return ref;
}