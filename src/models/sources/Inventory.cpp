#include "../headers/Inventory.h"

Inventory::Inventory() {
    slots.resize(9);
}

bool Inventory::addItem(const Item& item) {
    for (auto& slot : slots) {
        if (!slot.has_value()) {
            slot = item;
            return true;
        }
    }
    return false;
}

bool Inventory::addKeyFragment(const Item& fragment) {
    if (keyFragments.size() >= 3)
        return false;

    keyFragments.push_back(fragment);
    return true;
}

const std::vector<std::optional<Item>>& Inventory::getSlots() const {
    return slots;
}
// 🔹 NON-CONST VERSION
std::vector<std::optional<Item>>& Inventory::getSlots() {
    return slots;
}


const std::vector<Item>& Inventory::getKeyFragments() const {
    return keyFragments;
}
void Inventory::selectSlot(int const index) {
    if (index >= 0 && index <= 8) {
        selectedSlot = index;
    }
}

int Inventory::getSelectedSlot() const {
    return selectedSlot;
}

void Inventory::removeRandomItem() {
    // Get the index of not empty slots
    std::vector<int> filledSlots;
    for (int i =0; i< (int)slots.size(); i++) {
        if (slots[i].has_value()) {
            filledSlots.push_back(i);
        }
    }

    //if there is no items in player's inventory
    if (filledSlots.empty())
        return;

    //random pick
    int randomIndex = std::rand()%filledSlots.size();
    int slotToRemove = filledSlots[randomIndex];

    //remove the item
    slots[slotToRemove].reset();

    //ajust the selectef slot if it was the one who has benn removed
    if (selectedSlot == slotToRemove) {
        selectedSlot = 0;
    }
}

