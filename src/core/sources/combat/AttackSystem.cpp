#include "core/headers/combat/AttackSystem.h"
#include "core/headers/combat/ProjectileSystem.h"

// Handles player attacks (melee or projectile)
void AttackSystem::handleAttack(
    const AttackInfo& attack,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    ProjectileSystem& projectileSystem,
    sf::RectangleShape& debugMeleeBox,
    float& debugMeleeTimer
) {
    // Do nothing if the attack is not valid
    if (!attack.valid)
        return;

    // --- PROJECTILE ATTACK ---
    if (attack.isProjectile) {
        // Spawn a projectile with given parameters
        projectileSystem.spawn(
            attack.startPosition,
            attack.velocity,
            attack.damage,
            600.f
        );
    }
    else {
        // -------- MELEE ATTACK --------

        // --- DEBUG HITBOX ---
        // Display the melee hitbox for a short time
        debugMeleeBox.setPosition(
            attack.meleeHitbox.left,
            attack.meleeHitbox.top
        );
        debugMeleeBox.setSize({
            attack.meleeHitbox.width,
            attack.meleeHitbox.height
        });
        debugMeleeTimer = 0.1f;

        // --- DAMAGE ENEMIES ---
        for (auto& enemy : enemies) {
            // Skip dead enemies
            if (!enemy->isAlive()) continue;

            // Check collision between melee hitbox and enemy
            if (attack.meleeHitbox.intersects(enemy->getGlobalBounds())) {
                // Special case: percentage-based damage
                if (attack.damage == 0) {
                    enemy->takeDamage(
                        static_cast<int>(enemy->getHealth() * 0.75f)
                    );

                }
                else {
                    // Standard fixed damage
                    enemy->takeDamage(attack.damage);
                }
            }
        }
    }
}
