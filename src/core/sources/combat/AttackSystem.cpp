#include "core/headers/combat/AttackSystem.h"

#include "core/headers/combat/ProjectileSystem.h"

void AttackSystem::handleAttack(
    const AttackInfo& attack,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    ProjectileSystem& projectileSystem,
    sf::RectangleShape& debugMeleeBox,
    float& debugMeleeTimer
) {
    if (!attack.valid)
        return;

    if (attack.isProjectile) {
        projectileSystem.spawn(
            attack.startPosition,
            attack.velocity,
            attack.damage,
            600.f
        );
    }
    else {
        // -------- MELEE --------

        // DEBUG HITBOX
        debugMeleeBox.setPosition(
            attack.meleeHitbox.left,
            attack.meleeHitbox.top
        );
        debugMeleeBox.setSize({
            attack.meleeHitbox.width,
            attack.meleeHitbox.height
        });
        debugMeleeTimer = 0.1f;

        // DAMAGE ENEMIES
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            if (attack.meleeHitbox.intersects(enemy->getGlobalBounds())) {
                if (attack.damage == 0) {
                    enemy->takeDamage(
                        static_cast<int>(enemy->getHealth() * 0.75f)
                    );
                }
                else {
                    enemy->takeDamage(attack.damage);
                }
            }
        }
    }
}
