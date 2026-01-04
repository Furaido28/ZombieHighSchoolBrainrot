#pragma once

/* ==========================================================
 * Standard & SFML Includes
 * ========================================================== */
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

/* ==========================================================
 * Forward Declarations
 * (Reduce include dependencies)
 * ========================================================== */
class Enemy;
class Player;
class TileMap;

/* ==========================================================
 * EnemyController
 * Handles enemy AI, movement, and basic collision checks
 * ========================================================== */
class EnemyController {
public:
    /* =========================
     * Constructors
     * ========================= */
    EnemyController() = default;

    /* =========================
     * Update Logic
     * =========================
     * - Updates enemy movement and behavior
     * - Handles player tracking and interactions
     */
    void update(
        float dt,
        Player& player,
        const TileMap& map,
        std::vector<std::unique_ptr<Enemy>>& enemies
    );

private:
    /* ======================================================
     * Collision Helpers
     * ====================================================== */

    // Checks if a bounding box is free of map collisions
    bool isPositionFree(
        const sf::FloatRect& bbox,
        const TileMap& map
    ) const;

    // Checks intersection between two circular hitboxes
    bool circlesIntersect(
        const sf::Vector2f& aPos, float aRadius,
        const sf::Vector2f& bPos, float bRadius
    ) const;
};
