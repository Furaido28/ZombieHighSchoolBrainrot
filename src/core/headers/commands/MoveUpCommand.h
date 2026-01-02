#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

class MoveUpCommand : public Command {
public:
    MoveUpCommand(Player& player) : player(player) {}

    void execute(float dt) override {
        player.addMovement({0.f, -1.f});
        player.setDirection(Direction::Up);
        player.setMoving(true);
    }

private:
    Player& player;
};
