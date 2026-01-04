#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ProjectileSystem.h"
#include "models/headers/Enemy.h"
#include "models/headers/Player.h"
#include "models/headers/Player.h"
#include "core/headers/combat/Projectile.h"

class AttackSystem {
public:
    static void handleAttack(
        const AttackInfo& attack,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        ProjectileSystem& projectileSystem,
        sf::RectangleShape& debugMeleeBox,
        float& debugMeleeTimer
    );
};
