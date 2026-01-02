#pragma once
#include <SFML/System/Vector2.hpp>

class Boss;
class Player;

class BossPhase {
public:
    virtual ~BossPhase() = default;

    // Appelé UNE FOIS quand on entre dans la phase
    virtual void onEnter(Boss& boss) {}

    // Appelé UNE FOIS quand on quitte la phase
    virtual void onExit(Boss& boss) {}

    // Appelé À CHAQUE FRAME
    virtual void update(
        Boss& boss,
        float dt,
        const sf::Vector2f& playerPos
    ) = 0;

    // Dégâts infligés dans cette phase
    virtual int getDamage() const = 0;
};
