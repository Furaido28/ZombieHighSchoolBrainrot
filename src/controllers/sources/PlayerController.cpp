/* ==========================================================
 * Includes
 * ========================================================== */
#include "controllers/headers/PlayerController.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include <cmath>

/* ==========================================================
 * PlayerController::update
 * Handles player movement, direction, collisions and attacks
 * ========================================================== */
void PlayerController::update(
    float dt,
    Player& player,
    const TileMap& map,
    AttackInfo& outAttack
) {
    /* =========================
     * Movement Reset
     * ========================= */

    // Reset movement state
    player.setMoving(false);

    /* =========================
     * Player Internal Update
     * ========================= */

    // Update internal logic (cooldowns, timers, etc.)
    player.update(dt);

    /* =========================
     * Movement Direction
     * ========================= */

    // Retrieve accumulated movement direction
    sf::Vector2f dir = player.consumeMovement();

    if (dir.x != 0.f || dir.y != 0.f) {

        // Player is moving
        player.setMoving(true);

        // Update facing direction
        if (std::abs(dir.x) > std::abs(dir.y)) {
            player.setDirection(dir.x > 0 
                ? Direction::Right 
                : Direction::Left);
        } else {
            player.setDirection(dir.y > 0 
                ? Direction::Down 
                : Direction::Up);
        }

        // Normalize direction vector
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;
    }

    /* =========================
     * Movement & Collision
     * ========================= */

    // Compute movement delta
    sf::Vector2f delta = dir * 300.f * dt;

    // Current and future bounding boxes
    sf::FloatRect currentBBox = player.getGlobalBounds();
    sf::FloatRect futureBBox = currentBBox;
    futureBBox.left += delta.x;
    futureBBox.top  += delta.y;

    // Full movement possible
    if (isPositionFree(futureBBox, map)) {
        player.move(delta);
    }
    else {
        // Try sliding on X axis
        sf::FloatRect bboxX = currentBBox;
        bboxX.left += delta.x;

        if (isPositionFree(bboxX, map)) {
            player.move({ delta.x, 0.f });
        }
        else {
            // Try sliding on Y axis
            sf::FloatRect bboxY = currentBBox;
            bboxY.top += delta.y;

            if (isPositionFree(bboxY, map)) {
                player.move({ 0.f, delta.y });
            }
        }
    }

    /* =========================
     * Attack Decision
     * ========================= */

    // Compute attack once per update
    outAttack = player.tryAttack();
}

/* ==========================================================
 * PlayerController::isPositionFree
 * Checks collision against blocking tiles
 * ========================================================== */
bool PlayerController::isPositionFree(
    const sf::FloatRect& bbox,
    const TileMap& map
) const {

    // Tile size
    float tileSize = static_cast<float>(map.getTileSize());

    // Convert bounding box to tile coordinates
    int x0 = static_cast<int>(bbox.left / tileSize);
    int y0 = static_cast<int>(bbox.top / tileSize);
    int x1 = static_cast<int>((bbox.left + bbox.width - 0.001f) / tileSize);
    int y1 = static_cast<int>((bbox.top + bbox.height - 0.001f) / tileSize);

    // Check all tiles covered by the bounding box
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {

            // Retrieve tile type
            char t = map.getTile(x, y);

            // Blocking tiles
            if (t == '#' || t == 'R' || t == 'B' || t == 'A' ||
                t == 'r' || t == 'T' || t == '~')
                return false;
        }
    }

    // No collision detected
    return true;
}