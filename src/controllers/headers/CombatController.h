#pragma once

/* ==========================================================
 * SFML & Standard Includes
 * ========================================================== */
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

/* ==========================================================
 * Models
 * ========================================================== */
#include "models/headers/Player.h"
#include "models/headers/Enemy.h"
#include "models/headers/TileMap.h"

/* ==========================================================
 * Combat Systems
 * ========================================================== */
#include "core/headers/combat/ProjectileSystem.h"

/* ==========================================================
 * CombatController
 * Handles all combat-related logic (melee, projectiles, hits)
 * ========================================================== */
class CombatController {
public:
    /* =========================
     * Constructors
     * ========================= */
    CombatController();

    /* =========================
     * Combat Update
     * =========================
     * - Processes player attacks
     * - Updates projectiles
     * - Resolves enemy hits
     */
    void update(
        float dt,
        Player& player,
        const TileMap& map,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        const AttackInfo& attack
    );

    /* =========================
     * Rendering
     * =========================
     * Renders combat-related visuals
     * (projectiles, debug shapes, etc.)
     */
    void render(sf::RenderWindow& window);

private:
    /* ======================================================
     * Projectile System
     * ====================================================== */
    ProjectileSystem projectileSystem;

    /* ======================================================
     * Debug Visualization
     * ====================================================== */
    sf::RectangleShape debugMeleeBox;        // Shows melee hitbox
    sf::CircleShape debugProjectileRange;    // Shows projectile range
    float debugMeleeTimer = 0.f;             // Duration of melee debug display
    bool showProjectileRange = false;        // Toggle projectile debug

    /* ======================================================
     * Utility Methods
     * ====================================================== */

    // Checks if two circles intersect (used for collision detection)
    bool circlesIntersect(
        const sf::Vector2f& aPos, float aRadius,
        const sf::Vector2f& bPos, float bRadius
    ) const;
};
