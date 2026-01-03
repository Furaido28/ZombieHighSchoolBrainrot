#include "../headers/LuckyBoxSystem.h"
#include <cstdlib>

static const std::vector<WeightedLuckyOutcome> lootTable = {
    { LuckyOutcome::Heal, 30 },
    { LuckyOutcome::Medkit, 10 },
    { LuckyOutcome::Chalk, 40 },
    { LuckyOutcome::Pen, 20 },
    { LuckyOutcome::Book, 15 },
    { LuckyOutcome::Computer, 10 },
    { LuckyOutcome::LoseHealth, 30 },
    { LuckyOutcome::LoseRandomItem, 20 },
    {LuckyOutcome::Deo, 5}
};

LuckyOutcome LuckyBoxSystem::roll()
{
    int totalWeight = 0;
    for (const auto& entry : lootTable)
        totalWeight += entry.weight;

    int roll = std::rand() % totalWeight;
    int cumulative = 0;

    for (const auto& entry : lootTable) {
        cumulative += entry.weight;
        if (roll < cumulative)
            return entry.outcome;
    }

    // Sécurité (ne devrait jamais arriver)
    return LuckyOutcome::Heal;
}
