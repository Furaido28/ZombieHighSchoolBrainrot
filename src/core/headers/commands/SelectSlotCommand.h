#pragma once
#include "../Command.h"
#include "models/headers/Inventory.h"

// Command to directly select a specific inventory slot
class SelectSlotCommand : public Command {
public:
    // Constructor: Links command to inventory and the target slot number
    SelectSlotCommand(Inventory& inv, int slot)
        : inventory(inv), slot(slot) {}

    // Executes the switch
    void execute(float) override {
        inventory.selectSlot(slot);
    }

private:
    Inventory& inventory; // Reference to the inventory system
    int slot;             // The specific slot index to select
};