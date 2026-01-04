#pragma once

#include "Item.h"
#include <vector>
#include <optional>

class Inventory {
public:
    Inventory();

    bool addItem(const Item& item);
    bool addKeyFragment(const Item& fragment);

    const std::vector<std::optional<Item>>& getSlots() const;
    std::vector<std::optional<Item>>& getSlots();
    const std::vector<Item>& getKeyFragments() const;
    void selectSlot(int index);
    int getSelectedSlot() const;
    void removeRandomItem();

    void setExpanded(bool value) { expanded = value; }
    bool isExpanded() const { return expanded; }

private:
    std::vector<std::optional<Item>> slots; // 9 slots
    std::vector<Item> keyFragments;          // max 3
    int selectedSlot = 0; // 0,1,2 (hotbar)
    bool expanded = false;
};
