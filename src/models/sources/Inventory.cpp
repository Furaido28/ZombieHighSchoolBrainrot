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


