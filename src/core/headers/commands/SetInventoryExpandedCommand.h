#pragma once

#include "../Command.h"

class SetInventoryExpandedCommand : public Command {
public:
    SetInventoryExpandedCommand(bool& flag, bool value)
        : flag(flag), value(value) {}

    void execute(float) override {
        flag = value;
    }

private:
    bool& flag;
    bool value;
};
