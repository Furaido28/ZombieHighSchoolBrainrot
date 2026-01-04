#pragma once
#include <SFML/System/Vector2.hpp>

class Boss;
class Player;

// Base class for all boss phases
class BossPhase {
public:
    virtual ~BossPhase() = default;

    // Called ONCE when the boss enters this phase
    virtual void onEnter(Boss& boss) {}

    // Called ONCE when the boss leaves this phase
    virtual void onExit(Boss& boss) {}

    // Called EVERY FRAME while this phase is active
    virtual void update(
        Boss& boss,
        float dt,
        const sf::Vector2f& playerPos
    ) = 0;

    // Returns the damage dealt during this phase
    virtual int getDamage() const = 0;
};
