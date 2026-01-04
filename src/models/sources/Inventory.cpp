#include "../headers/Inventory.h"

// Constructor: creates an inventory with a fixed number of slots
Inventory::Inventory() {
    // Initialize 9 empty slots
    slots.resize(9);
}

// Adds an item to the first available empty slot
bool Inventory::addItem(const Item& item) {
    for (auto& slot : slots) {
        // Check for an empty slot
        if (!slot.has_value()) {
            slot = item;
            return true;
        }
    }
    // Inventory is full
    return false;
}

// Adds a key fragment (maximum of 3 allowed)
bool Inventory::addKeyFragment(const Item& fragment) {
    // Do not exceed the maximum number of key fragments
    if (keyFragments.size() >= 3)
        return false;

    keyFragments.push_back(fragment);
    return true;
}

// Returns inventory slots (read-only access)
const std::vector<std::optional<Item>>& Inventory::getSlots() const {
    return slots;
}

// Returns inventory slots (modifiable access)
std::vector<std::optional<Item>>& Inventory::getSlots() {
    return slots;
}

// Returns collected key fragments
const std::vector<Item>& Inventory::getKeyFragments() const {
    return keyFragments;
}

// Selects an inventory slot by index
void Inventory::selectSlot(int const index) {
    // Ensure index is inside valid range
    if (index >= 0 && index <= 8) {
        selectedSlot = index;
    }
}

// Returns the currently selected slot index
int Inventory::getSelectedSlot() const {
    return selectedSlot;
}

// Removes a random item from the inventory
void Inventory::removeRandomItem() {
    // Collect indices of all non-empty slots
    std::vector<int> filledSlots;
    for (int i = 0; i < (int)slots.size(); i++) {
        if (slots[i].has_value()) {
            filledSlots.push_back(i);
        }
    }

    // If inventory is empty, do nothing
    if (filledSlots.empty())
        return;

    // Pick a random filled slot
    int randomIndex = std::rand() % filledSlots.size();
    int slotToRemove = filledSlots[randomIndex];

    // Remove the item from the selected slot
    slots[slotToRemove].reset();

    // Reset selected slot if the removed one was selected
    if (selectedSlot == slotToRemove) {
        selectedSlot = 0;
    }
}