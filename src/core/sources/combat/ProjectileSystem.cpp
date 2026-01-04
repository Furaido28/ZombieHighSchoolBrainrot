#include "core/headers/combat/ProjectileSystem.h"

#include "models/headers/Enemy.h"
#include "models/headers/TileMap.h"

#include <algorithm>
#include <cmath>

// =========================
// SPAWN
// =========================
void ProjectileSystem::spawn(const sf::Vector2f& startPos,
                             const sf::Vector2f& velocity,
                             int damage,
                             float maxRange)
{
    Projectile p;
    p.position = startPos;
    p.velocity = velocity;
    p.damage = damage;
    p.maxRange = maxRange;

    p.shape.setRadius(4.f);
    p.shape.setOrigin(4.f, 4.f);
    p.shape.setFillColor(sf::Color::White);
    p.shape.setPosition(p.position);

    projectiles.push_back(p);
}

// =========================
// UPDATE
// =========================
void ProjectileSystem::update(float dt,
                              const TileMap& map,
                              std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (auto& p : projectiles) {
        if (!p.active) continue;

        // Déplacement
        sf::Vector2f move = p.velocity * dt;
        p.position += move;
        p.distanceTraveled += std::sqrt(move.x * move.x + move.y * move.y);
        p.shape.setPosition(p.position);

        // -------------------------
        // Collision MAP
        // -------------------------
        sf::FloatRect bbox(
            p.position.x - 2.f,
            p.position.y - 2.f,
            4.f,
            4.f
        );

        if (!isPositionFree(bbox, map)) {
            p.active = false;
            continue;
        }

        // -------------------------
        // Portée max
        // -------------------------
        if (p.distanceTraveled >= p.maxRange) {
            p.active = false;
            continue;
        }

        // -------------------------
        // Collision ENNEMIS
        // -------------------------
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            if (enemy->getGlobalBounds().contains(p.position)) {
                enemy->takeDamage(p.damage);
                p.active = false;
                break;
            }
        }
    }

    // Nettoyage
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.active; }),
        projectiles.end());
}

// =========================
// RENDER
// =========================
void ProjectileSystem::render(sf::RenderWindow& window)
{
    for (const auto& p : projectiles) {
        window.draw(p.shape);
    }
}

// =========================
// CLEAR
// =========================
void ProjectileSystem::clear()
{
    projectiles.clear();
}

// =========================
// COLLISION MAP
// =========================
bool ProjectileSystem::isPositionFree(const sf::FloatRect& bbox,
                                      const TileMap& map) const
{
    float tileSize = static_cast<float>(map.getTileSize());

    int x0 = static_cast<int>(std::floor(bbox.left / tileSize));
    int y0 = static_cast<int>(std::floor(bbox.top / tileSize));
    int x1 = static_cast<int>(std::floor((bbox.left + bbox.width) / tileSize));
    int y1 = static_cast<int>(std::floor((bbox.top + bbox.height) / tileSize));

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            char tile = map.getTile(x, y);
            if (tile == '#' || tile == 'R' || tile == 'B' ||
                tile == 'A' || tile == 'r' || tile == 'T' || tile == '~')
            {
                return false;
            }
        }
    }
    return true;
}
