#pragma once

#include "../Command.h"
#include "models/headers/Inventory.h"

// Command to select the previous item slot
class PrevSlotCommand : public Command {
public:
    // Constructor
    PrevSlotCommand(Inventory& inv)
        : inv(inv) {}

    // Execute logic to switch slots
    void execute(float) override {
        int cur = inv.getSelectedSlot();

        // Only move back if we aren't at the first slot (0)
        if (cur > 0) inv.selectSlot(cur - 1);
    }

private:
    Inventory& inv; // Reference to the inventory
};