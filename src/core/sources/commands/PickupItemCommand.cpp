#include "core/headers/commands/PickupItemCommand.h"
#include "controllers/headers/GameController.h"
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

void PickupItemCommand::execute(float)
{
    for (auto it = worldItems.begin(); it != worldItems.end();) {

        sf::FloatRect bounds = it->item.sprite.getGlobalBounds();
        sf::Vector2f itemCenter(
            bounds.left + bounds.width / 2.f,
            bounds.top  + bounds.height / 2.f
        );

        if (circlesIntersect(
            player.getPosition(), player.getRadius(),
            itemCenter, it->radius))
        {
            //===================
            // --- LUCKY BOX ---
            //===================
            if (it->item.type == ItemType::LuckyBox) {
                int index = std::distance(worldItems.begin(), it);
                controller.openLuckyBox(index);
                return;
            }

            bool picked = false;

            if (it->item.type == ItemType::KeyFragment)
                picked = player.getInventory().addKeyFragment(it->item);
            if (picked) {
                if (it->item.type == ItemType::KeyFragment) {
                    controller.onKeyFragmentPicked();
                }
                it = worldItems.erase(it);
                continue;
            }
            else
                picked = player.getInventory().addItem(it->item);

            if (picked) {
                it = worldItems.erase(it);
                continue;
            }
        }
        ++it;
    }
}
