#include "core/headers/commands/AttackCommand.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>

static bool circlesIntersect(
    const sf::Vector2f& aPos, float aRadius,
    const sf::Vector2f& bPos, float bRadius)
{
    sf::Vector2f d = aPos - bPos;
    float distSq = d.x * d.x + d.y * d.y;
    float r = aRadius + bRadius;
    return distSq <= r * r;
}

void AttackCommand::execute(float) {
    if (attackTimer > 0.f)
        return;

    int slot = player.getInventory().getSelectedSlot();
    auto& slots = player.getInventory().getSlots();

    if (!slots[slot].has_value())
        return;

    Item& item = slots[slot].value();

    if (item.type != ItemType::Weapon)
        return;

    bool hit = false;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive())
            continue;

        if (circlesIntersect(
            player.getPosition(), 80.f,
            enemy->getPosition(), enemy->getRadius()))
        {
            enemy->takeDamage(item.value);
            hit = true;
        }
    }

    if (hit)
        attackTimer = attackCooldown;
}
