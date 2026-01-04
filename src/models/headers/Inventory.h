#pragma once

#include "Item.h"
#include <vector>
#include <optional>

// Player inventory system
class Inventory {
public:
    // Constructor: initializes the inventory
    Inventory();

    // Adds an item to the first available slot
    bool addItem(const Item& item);

    // Adds a key fragment (maximum of 3 allowed)
    bool addKeyFragment(const Item& fragment);

    // Access to inventory slots (read-only)
    const std::vector<std::optional<Item>>& getSlots() const;

    // Access to inventory slots (modifiable)
    std::vector<std::optional<Item>>& getSlots();

    // Returns collected key fragments
    const std::vector<Item>& getKeyFragments() const;

    // Selects an active slot by index
    void selectSlot(int index);

    // Returns the currently selected slot
    int getSelectedSlot() const;

    // Removes a random item from the inventory
    void removeRandomItem();

    // Inventory UI state (expanded / collapsed)
    void setExpanded(bool value) { expanded = value; }
    bool isExpanded() const { return expanded; }

private:
    // Item slots (fixed size: 9)
    std::vector<std::optional<Item>> slots;

    // Special items used for progression (max 3)
    std::vector<Item> keyFragments;

    // Currently selected slot (hotbar)
    int selectedSlot = 0;

    // Indicates whether the inventory UI is expanded
    bool expanded = false;
};
