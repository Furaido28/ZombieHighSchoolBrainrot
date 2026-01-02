#include "models/headers/Enemy/EnemyFlyweightFactory.h"

const EnemyArchetype& EnemyFlyweightFactory::get(EnemyType type) {
    auto it = cache.find(type);
    if (it != cache.end()) return *it->second;

    auto a = std::make_unique<EnemyArchetype>();
    a->type = type;

    // Valeurs par défaut (tu peux ajuster)
    switch (type) {
        case EnemyType::Basic:
            a->speed = 100.f;
            a->maxHealth = 50.f;
            a->damage = 10;
            a->attackCooldown = 1.f;
            a->renderRadius = 50.f;
            a->collisionRadius = 24.f;
            break;

        case EnemyType::Fast:
            a->speed = 160.f;
            a->maxHealth = 35.f;
            a->damage = 8;
            a->attackCooldown = 0.8f;
            a->renderRadius = 48.f;
            a->collisionRadius = 22.f;
            break;

        case EnemyType::Tank:
            a->speed = 70.f;
            a->maxHealth = 120.f;
            a->damage = 18;
            a->attackCooldown = 1.2f;
            a->renderRadius = 60.f;
            a->collisionRadius = 28.f;
            break;

        default:
            // Boss gérés ailleurs, mais on met un fallback
            a->speed = 100.f;
            a->maxHealth = 50.f;
            a->damage = 10;
            a->attackCooldown = 1.f;
            a->renderRadius = 50.f;
            a->collisionRadius = 24.f;
            break;
    }

    auto& ref = *a;
    cache.emplace(type, std::move(a));
    return ref;
}
