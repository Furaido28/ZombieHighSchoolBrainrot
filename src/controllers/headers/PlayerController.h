#pragma once

/* ==========================================================
 * SFML Includes
 * ========================================================== */
#include <SFML/Graphics.hpp>

/* ==========================================================
 * Forward Declarations
 * ========================================================== */
class Player;
class TileMap;
struct AttackInfo;

/* ==========================================================
 * PlayerController
 * Handles player movement, actions, and attack decisions
 * ========================================================== */
class PlayerController {
public:
    /* =========================
     * Update Logic
     * =========================
     * - Processes player movement
     * - Detects attacks and fills AttackInfo
     */
    void update(
        float dt,
        Player& player,
        const TileMap& map,
        AttackInfo& outAttack
    );

private:
    /* ======================================================
     * Collision Helper
     * ====================================================== */

    // Checks if a player bounding box is free of map collisions
    bool isPositionFree(
        const sf::FloatRect& bbox,
        const TileMap& map
    ) const;
};
