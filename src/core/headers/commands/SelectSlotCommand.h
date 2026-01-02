#pragma once
#include "../Command.h"
#include "models/headers/Inventory.h"

class SelectSlotCommand : public Command {
public:
    SelectSlotCommand(Inventory& inv, int slot)
        : inventory(inv), slot(slot) {}

    void execute(float) override {
        inventory.selectSlot(slot);
    }

private:
    Inventory& inventory;
    int slot;
};
