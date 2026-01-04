#include "controllers/headers/CombatController.h"
#include "core/headers/combat/AttackSystem.h"
#include <cmath>

CombatController::CombatController() {
    debugMeleeBox.setFillColor(sf::Color(255, 0, 0, 100));

    debugProjectileRange.setFillColor(sf::Color::Transparent);
    debugProjectileRange.setOutlineColor(sf::Color(0, 100, 255, 150));
    debugProjectileRange.setOutlineThickness(2.f);
}

bool CombatController::circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius
) const {
    sf::Vector2f d = aPos - bPos;
    float distSq = d.x * d.x + d.y * d.y;
    float r = aRadius + bRadius;
    return distSq <= r * r;
}

void CombatController::update(
    float dt,
    Player& player,
    const TileMap& map,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    const AttackInfo& attack
) {
    // =========================
    // DEBUG TIMER
    // =========================
    if (debugMeleeTimer > 0.f)
        debugMeleeTimer -= dt;

    // =========================
    // PROJECTILE RANGE PREVIEW
    // =========================
    showProjectileRange = false;
    int slot = player.getInventory().getSelectedSlot();
    auto& slots = player.getInventory().getSlots();

    if (slot >= 0 && slot < (int)slots.size() && slots[slot].has_value()) {
        const Item& item = slots[slot].value();
        if (item.type == ItemType::Weapon && item.isProjectile) {
            showProjectileRange = true;
            debugProjectileRange.setRadius(item.range);
            debugProjectileRange.setOrigin(item.range, item.range);
            debugProjectileRange.setPosition(player.getPosition());
        }
    }

    // =========================
    // ENEMY ATTACK PLAYER
    // =========================
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            enemy->getPosition(), enemy->getRadius()
        )) {
            enemy->attack(player);
        }
    }

    // =========================
    // PLAYER ATTACK
    // =========================
    if (attack.valid) {
        AttackSystem::handleAttack(
            attack,
            enemies,
            projectileSystem,
            debugMeleeBox,
            debugMeleeTimer
        );
    }

    // =========================
    // PROJECTILES UPDATE
    // =========================
    projectileSystem.update(dt, map, enemies);
}

void CombatController::render(sf::RenderWindow& window) {
    projectileSystem.render(window);

    if (showProjectileRange)
        window.draw(debugProjectileRange);

    if (debugMeleeTimer > 0.f)
        window.draw(debugMeleeBox);
}