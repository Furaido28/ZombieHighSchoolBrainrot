#pragma once

#include "core/headers/Command.h"

class Player;
class WorldItemSystem;
class GameController;

class PickupItemCommand : public Command {
public:
    PickupItemCommand(
        Player& player,
        WorldItemSystem& worldItemSystem,
        GameController& game
    );

    void execute(float dt) override;

private:
    Player& player;
    WorldItemSystem& worldItemSystem;
    GameController& game;
};
