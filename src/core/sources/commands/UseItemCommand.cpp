#include "core/headers/commands/UseItemCommand.h"
#include "models/headers/Item.h"

void UseItemCommand::execute(float)
{
    auto& inventory = player.getInventory();
    int slot = inventory.getSelectedSlot();
    auto& slots = inventory.getSlots();

    if (!slots[slot].has_value())
        return;

    Item& item = slots[slot].value();

    if (item.type == ItemType::Consumable) {
        int newHp = player.getHealth() + item.value;
        player.setHealth(std::min(newHp, player.getMaxHealth()));
        slots[slot].reset();
    }
}
