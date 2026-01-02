#pragma once

#include "../Command.h"
#include "models/headers/Player.h"

class AttackCommand : public Command {
public:
    explicit AttackCommand(Player& player)
        : player(player) {}

    void execute(float dt) override;

private:
    Player& player;
};
