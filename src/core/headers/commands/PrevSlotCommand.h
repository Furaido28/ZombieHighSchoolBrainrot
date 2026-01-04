#pragma once

#include "../Command.h"
#include "models/headers/Inventory.h"

class PrevSlotCommand : public Command {
public:
    PrevSlotCommand(Inventory& inv)
        : inv(inv) {}

    void execute(float) override {
        int cur = inv.getSelectedSlot();
        if (cur > 0) inv.selectSlot(cur - 1);
    }

private:
    Inventory& inv;
};
