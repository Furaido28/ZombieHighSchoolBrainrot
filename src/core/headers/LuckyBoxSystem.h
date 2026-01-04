#pragma once
#include <vector>

// List of all possible results when opening a Lucky Box
enum class LuckyOutcome {
    Heal,            // Restore health immediately
    Medkit,          // Receive a Medkit item
    Chalk,           // Receive Chalk
    Pen,             // Receive Pen
    Book,            // Receive Book
    Computer,        // Receive Computer
    LoseHealth,      // Bad luck: Player takes damage
    LoseRandomItem,  // Bad luck: An item is removed from inventory
    Deo              // Receive Deodorant
};

// Helper struct to link an outcome with its probability (weight)
struct WeightedLuckyOutcome {
    LuckyOutcome outcome;
    int weight; // Higher weight = higher chance to get this outcome
};

// Handle the random logic of the Lucky Box
class LuckyBoxSystem {
public:
    // Generates a random result based on the defined weights
    static LuckyOutcome roll();
};