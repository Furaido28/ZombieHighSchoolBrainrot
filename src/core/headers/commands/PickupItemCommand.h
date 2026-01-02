#pragma once

#include "../Command.h"
#include "models/headers/Player.h"
#include "models/headers/Item.h"
#include <vector>

struct WorldItem;

class PickupItemCommand : public Command {
public:
    PickupItemCommand(Player& player,
                      std::vector<WorldItem>& worldItems)
        : player(player), worldItems(worldItems) {}

    void execute(float dt) override;

private:
    Player& player;
    std::vector<WorldItem>& worldItems;
};
