#include "controllers/headers/PlayerController.h"
#include "models/headers/Player.h"
#include "models/headers/TileMap.h"
#include <cmath>

void PlayerController::update(
    float dt,
    Player& player,
    const TileMap& map,
    AttackInfo& outAttack
) {
    // Reset mouvement
    player.setMoving(false);

    // Update logique interne (cooldowns etc.)
    player.update(dt);

    // Récupération direction
    sf::Vector2f dir = player.consumeMovement();

    if (dir.x != 0.f || dir.y != 0.f) {
        player.setMoving(true);

        if (std::abs(dir.x) > std::abs(dir.y)) {
            player.setDirection(dir.x > 0 ? Direction::Right : Direction::Left);
        } else {
            player.setDirection(dir.y > 0 ? Direction::Down : Direction::Up);
        }

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= len;
    }

    sf::Vector2f delta = dir * 300.f * dt;

    sf::FloatRect currentBBox = player.getGlobalBounds();
    sf::FloatRect futureBBox = currentBBox;
    futureBBox.left += delta.x;
    futureBBox.top  += delta.y;

    if (isPositionFree(futureBBox, map)) {
        player.move(delta);
    }
    else {
        sf::FloatRect bboxX = currentBBox;
        bboxX.left += delta.x;

        if (isPositionFree(bboxX, map)) {
            player.move({ delta.x, 0.f });
        }
        else {
            sf::FloatRect bboxY = currentBBox;
            bboxY.top += delta.y;

            if (isPositionFree(bboxY, map)) {
                player.move({ 0.f, delta.y });
            }
        }
    }

    // Décision d’attaque (UNE FOIS)
    outAttack = player.tryAttack();
}

bool PlayerController::isPositionFree(
    const sf::FloatRect& bbox,
    const TileMap& map
) const {
    float tileSize = static_cast<float>(map.getTileSize());

    int x0 = static_cast<int>(bbox.left / tileSize);
    int y0 = static_cast<int>(bbox.top / tileSize);
    int x1 = static_cast<int>((bbox.left + bbox.width - 0.001f) / tileSize);
    int y1 = static_cast<int>((bbox.top + bbox.height - 0.001f) / tileSize);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char t = map.getTile(x, y);
            if (t == '#' || t=='R' || t=='B' || t=='A' || t=='r' || t=='T' || t=='~')
                return false;
        }
    }
    return true;
}