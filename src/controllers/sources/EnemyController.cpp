#include "controllers/headers/EnemyController.h"
#include "models/headers/Enemy.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include <cmath>

void EnemyController::update(
    float dt,
    Player& player,
    const TileMap& map,
    std::vector<std::unique_ptr<Enemy>>& enemies
) {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive())
            continue;

        sf::Vector2f oldPos = enemy->getPosition();

        // IA basique : suivre le joueur
        enemy->update(dt, player.getPosition());

        sf::Vector2f newPos = enemy->getPosition();
        sf::Vector2f delta = newPos - oldPos;

        // Reset position (collision check)
        enemy->setPosition(oldPos);

        sf::FloatRect bbox = enemy->getGlobalBounds();
        sf::FloatRect future = bbox;
        future.left += delta.x;
        future.top  += delta.y;

        // Collision map
        if (isPositionFree(future, map)) {
            enemy->setPosition(oldPos + delta);
        }
        else {
            // Glissement X
            sf::FloatRect bboxX = bbox;
            bboxX.left += delta.x;

            if (isPositionFree(bboxX, map)) {
                enemy->setPosition({ oldPos.x + delta.x, oldPos.y });
            }
            else {
                // Glissement Y
                sf::FloatRect bboxY = bbox;
                bboxY.top += delta.y;

                if (isPositionFree(bboxY, map)) {
                    enemy->setPosition({ oldPos.x, oldPos.y + delta.y });
                }
                else {
                    enemy->setPosition(oldPos);
                }
            }
        }

        // Attaque joueur
        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            enemy->getPosition(), enemy->getRadius()
        )) {
            enemy->attack(player);
        }
    }
}

bool EnemyController::isPositionFree(
    const sf::FloatRect& bbox,
    const TileMap& map
) const {
    float tileSize = (float)map.getTileSize();

    int x0 = (int)std::floor(bbox.left / tileSize);
    int y0 = (int)std::floor(bbox.top / tileSize);
    int x1 = (int)std::floor((bbox.left + bbox.width  - 0.001f) / tileSize);
    int y1 = (int)std::floor((bbox.top  + bbox.height - 0.001f) / tileSize);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char t = map.getTile(x, y);
            if (t == '#' || t=='R' || t=='B' || t=='A' || t=='r' || t=='T' || t=='~')
                return false;
        }
    }
    return true;
}

bool EnemyController::circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius
) const {
    sf::Vector2f d = aPos - bPos;
    float distSq = d.x * d.x + d.y * d.y;
    float r = aRadius + bRadius;
    return distSq <= r * r;
}
