#pragma once

#include "core/headers/InputHandler.h"
#include "core/headers/commands/PrevSlotCommand.h"
#include "core/headers/commands/NextSlotCommand.h"

class Player;
class Inventory;
class WorldItemSystem;
class GameController;

class InputController {
public:
    InputController(
        Player& player,
        Inventory& inventory,
        WorldItemSystem& worldItemSystem,
        GameController& game
    );

    void handleEvent(const sf::Event& event);
    void update(float dt);

private:
    InputHandler inputHandler;

    Player& player;
    Inventory& inventory;
    WorldItemSystem& worldItemSystem;
    GameController& game;

    bool tabPressed = false;

    std::unique_ptr<NextSlotCommand> nextSlotCommand;
    std::unique_ptr<PrevSlotCommand> prevSlotCommand;
};
