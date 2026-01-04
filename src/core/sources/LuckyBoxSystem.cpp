#include "../headers/LuckyBoxSystem.h"
#include <cstdlib>

// Loot table with weighted probabilities
// Higher weight = higher chance to be selected
static const std::vector<WeightedLuckyOutcome> lootTable = {
    { LuckyOutcome::Heal, 30 },
    { LuckyOutcome::Medkit, 10 },
    { LuckyOutcome::Chalk, 40 },
    { LuckyOutcome::Pen, 20 },
    { LuckyOutcome::Book, 15 },
    { LuckyOutcome::Computer, 10 },
    { LuckyOutcome::LoseHealth, 30 },
    { LuckyOutcome::LoseRandomItem, 20 },
    { LuckyOutcome::Deo, 5 }
};

// Rolls a random outcome based on weights
LuckyOutcome LuckyBoxSystem::roll()
{
    // Compute total weight of all outcomes
    int totalWeight = 0;
    for (const auto& entry : lootTable)
        totalWeight += entry.weight;

    // Generate a random value within the total weight
    int roll = std::rand() % totalWeight;
    int cumulative = 0;

    // Find which outcome matches the random roll
    for (const auto& entry : lootTable) {
        cumulative += entry.weight;
        if (roll < cumulative)
            return entry.outcome;
    }

    // Safety fallback (should never happen)
    return LuckyOutcome::Heal;
}