#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

class MoveRightCommand : public Command {
public:
    MoveRightCommand(Player& player) : player(player) {}

    void execute(float dt) override {
        player.addMovement({1.f, 0.f});
        player.setDirection(Direction::Right);
        player.setMoving(true);

    }

private:
    Player& player;

};
