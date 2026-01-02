#pragma once

#include "../Command.h"

#include "models/headers/Player.h"

class MoveLeftCommand : public Command {
public:
    MoveLeftCommand(Player& player) : player(player) {}

    void execute(float dt) override {
        player.addMovement({-1.f, 0.f});
        player.setDirection(Direction::Left);
        player.setMoving(true);
    }

private:
    Player& player;

};