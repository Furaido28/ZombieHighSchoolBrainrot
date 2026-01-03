#pragma once
#include <vector>

enum class LuckyOutcome {
    Heal,
    Medkit,
    Chalk,
    Pen,
    Book,
    Computer,
    LoseHealth,
    LoseRandomItem,
    Deo
};

struct WeightedLuckyOutcome {
    LuckyOutcome outcome;
    int weight;
};

class LuckyBoxSystem {
public:
    static LuckyOutcome roll();
};
