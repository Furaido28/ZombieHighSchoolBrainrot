#pragma once

#include "../Command.h"

// Command to change the state of the inventory (open or close)
class SetInventoryExpandedCommand : public Command {
public:
    // Constructor
    SetInventoryExpandedCommand(bool& flag, bool value)
        : flag(flag), value(value) {}

    // Updates the flag immediately
    void execute(float) override {
        flag = value;
    }

private:
    bool& flag;
    bool value;
};