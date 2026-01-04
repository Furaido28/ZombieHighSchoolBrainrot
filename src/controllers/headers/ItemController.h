#pragma once

#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"

class GameController;

class ItemController {
public:
    ItemController(WorldItemSystem& worldItems, GameController& game);

    void tryPickup(Player& player);

private:
    WorldItemSystem& worldItemSystem;
    GameController& game;
};
