#include "core/headers/commands/UseItemCommand.h"
#include "models/headers/Item.h"

// Command that uses the currently selected inventory item
void UseItemCommand::execute(float)
{
    // Access the player's inventory
    auto& inventory = player.getInventory();
    int slot = inventory.getSelectedSlot();
    auto& slots = inventory.getSlots();

    // Do nothing if the selected slot is empty
    if (!slots[slot].has_value())
        return;

    // Get the selected item
    Item& item = slots[slot].value();

    // Only consumable items can be used here
    if (item.type == ItemType::Consumable) {
        // Heal the player
        int newHp = player.getHealth() + item.value;
        player.setHealth(std::min(newHp, player.getMaxHealth()));

        // Remove the item after use
        slots[slot].reset();
    }
}
