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
    // Main function to process an attack
    // It checks if enemies are hit
    static void handleAttack(
        const AttackInfo& attack,                       // Data regarding the attack (damage, range, type)
        std::vector<std::unique_ptr<Enemy>>& enemies,   // List of active enemies to check for hits
        ProjectileSystem& projectileSystem,             // System to spawn projectiles (if ranged)
        sf::RectangleShape& debugMeleeBox,              // Visual shape to see the melee hitbox
        float& debugMeleeTimer                          // Timer to keep the box visible briefly
    );
};
