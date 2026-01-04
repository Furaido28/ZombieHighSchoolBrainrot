/* ==========================================================
 * Includes
 * ========================================================== */
#include "controllers/headers/EnemyController.h"
#include "models/headers/Enemy.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include <cmath>

/* ==========================================================
 * EnemyController::update
 * Updates all enemies:
 *  - Movement toward the player
 *  - Collision handling with the map
 *  - Player attack detection
 * ========================================================== */
void EnemyController::update(
    float dt,
    Player& player,
    const TileMap& map,
    std::vector<std::unique_ptr<Enemy>>& enemies
) {
    // Loop through all enemies
    for (auto& enemy : enemies) {

        // Skip dead enemies
        if (!enemy->isAlive())
            continue;

        // Save old position for collision rollback
        sf::Vector2f oldPos = enemy->getPosition();

        /* =========================
         * Basic AI
         * ========================= */

        // Simple AI: enemy moves toward the player
        enemy->update(dt, player.getPosition());

        // Compute movement delta
        sf::Vector2f newPos = enemy->getPosition();
        sf::Vector2f delta = newPos - oldPos;

        // Reset position before collision checks
        enemy->setPosition(oldPos);

        /* =========================
         * Collision Detection
         * ========================= */

        // Current bounding box
        sf::FloatRect bbox = enemy->getGlobalBounds();

        // Future bounding box (full movement)
        sf::FloatRect future = bbox;
        future.left += delta.x;
        future.top  += delta.y;

        // Check collision against the map
        if (isPositionFree(future, map)) {
            // Full movement allowed
            enemy->setPosition(oldPos + delta);
        }
        else {
            /* =========================
             * Sliding Logic
             * ========================= */

            // Try sliding on X axis
            sf::FloatRect bboxX = bbox;
            bboxX.left += delta.x;

            if (isPositionFree(bboxX, map)) {
                enemy->setPosition({ oldPos.x + delta.x, oldPos.y });
            }
            else {
                // Try sliding on Y axis
                sf::FloatRect bboxY = bbox;
                bboxY.top += delta.y;

                if (isPositionFree(bboxY, map)) {
                    enemy->setPosition({ oldPos.x, oldPos.y + delta.y });
                }
                else {
                    // No movement possible
                    enemy->setPosition(oldPos);
                }
            }
        }
        /* =========================
         * Player Attack
         * ========================= */

        float dist = std::hypot(
            player.getPosition().x - enemy->getPosition().x,
            player.getPosition().y - enemy->getPosition().y
        );
        constexpr float ATTACK_DISTANCE = 10.f;

        if (dist <= ATTACK_DISTANCE) {
            enemy->attack(player);
        }
    }
}

/* ==========================================================
 * EnemyController::isPositionFree
 * Checks if a bounding box collides with blocking tiles
 * ========================================================== */
bool EnemyController::isPositionFree(
    const sf::FloatRect& bbox,
    const TileMap& map
) const {
    // Retrieve tile size
    float tileSize = (float)map.getTileSize();

    // Convert bounding box to tile coordinates
    int x0 = (int)std::floor(bbox.left / tileSize);
    int y0 = (int)std::floor(bbox.top / tileSize);
    int x1 = (int)std::floor((bbox.left + bbox.width  - 0.001f) / tileSize);
    int y1 = (int)std::floor((bbox.top  + bbox.height - 0.001f) / tileSize);

    // Check all tiles covered by the bounding box
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {

            // Get tile type
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

/* ==========================================================
 * EnemyController::circlesIntersect
 * Checks collision between two circles
 * ========================================================== */
bool EnemyController::circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius
) const {
    // Vector between centers
    sf::Vector2f d = aPos - bPos;

    // Squared distance
    float distSq = d.x * d.x + d.y * d.y;

    // Sum of radii
    float r = aRadius + bRadius;

    // Collision test
    return distSq <= r * r;
}