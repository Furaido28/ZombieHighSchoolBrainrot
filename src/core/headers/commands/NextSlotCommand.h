#pragma once

#include "../Command.h"
#include "models/headers/Inventory.h"

// Command to select the next item slot
class NextSlotCommand : public Command {
public:
    // Constructor
    NextSlotCommand(Inventory& inv, bool& tabPressed)
        : inv(inv), tabPressed(tabPressed) {}

    // Execute logic to switch slots
    void execute(float) override {
        // If Tab is held, access full inventory
        int maxSlot = tabPressed ? 8 : 2;

        int cur = inv.getSelectedSlot();

        // Move to the next slot only if we aren't at the limit
        if (cur < maxSlot)
            inv.selectSlot(cur + 1);
    }

private:
    Inventory& inv;   // Reference to the inventory system
    bool& tabPressed; // Reference to check if Tab is currently held down
};