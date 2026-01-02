#pragma once

#include "../Command.h"
#include "models/headers/Player.h"

class UseItemCommand : public Command {
public:
    explicit UseItemCommand(Player& player)
        : player(player) {}

    void execute(float dt) override;

private:
    Player& player;

};
