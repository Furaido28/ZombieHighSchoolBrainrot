#pragma once
#include <memory>
#include <unordered_map>
#include "models/headers/Enemy.h"
#include "models/headers/Enemy/EnemyArchetype.h"

// Factory responsible for creating and reusing enemy archetypes
// Implements the Flyweight pattern
class EnemyFlyweightFactory {
public:
    // Returns a shared archetype for the given enemy type
    const EnemyArchetype& get(EnemyType type);

private:
    // Cache storing one archetype per enemy type
    std::unordered_map<EnemyType, std::unique_ptr<EnemyArchetype>> cache;
};
