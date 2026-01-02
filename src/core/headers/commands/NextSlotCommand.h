#pragma once

#include "../Command.h"

class NextSlotCommand : public Command {
public:
    NextSlotCommand(Inventory& inv, bool& tabPressed)
        : inv(inv), tabPressed(tabPressed) {}

    void execute(float) override {
        int maxSlot = tabPressed ? 8 : 2;
        int cur = inv.getSelectedSlot();
        if (cur < maxSlot) inv.selectSlot(cur + 1);
    }

private:
    Inventory& inv;
    bool& tabPressed;
};
