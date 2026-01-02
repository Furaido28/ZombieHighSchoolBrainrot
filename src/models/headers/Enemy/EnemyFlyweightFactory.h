#pragma once
#include <memory>
#include <unordered_map>
#include "models/headers/Enemy.h"
#include "models/headers/Enemy/EnemyArchetype.h"

class EnemyFlyweightFactory {
public:
    const EnemyArchetype& get(EnemyType type);

private:
    std::unordered_map<EnemyType, std::unique_ptr<EnemyArchetype>> cache;
};
