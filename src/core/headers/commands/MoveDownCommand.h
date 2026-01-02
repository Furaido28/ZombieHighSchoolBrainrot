#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

class MoveDownCommand : public Command {
public:
    MoveDownCommand(Player& player) : player(player) {}

    void execute(float dt) override {
        player.addMovement({0.f, 1.f});
        player.setDirection(Direction::Down);
        player.setMoving(true);
    }

private:
    Player& player;

};
