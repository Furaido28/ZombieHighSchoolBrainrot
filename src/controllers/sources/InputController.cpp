#include "controllers/headers/InputController.h"

#include "core/headers/commands/MoveUpCommand.h"
#include "core/headers/commands/MoveDownCommand.h"
#include "core/headers/commands/MoveLeftCommand.h"
#include "core/headers/commands/MoveRightCommand.h"
#include "core/headers/commands/AttackCommand.h"
#include "core/headers/commands/PickupItemCommand.h"
#include "core/headers/commands/SelectSlotCommand.h"
#include "core/headers/commands/UseItemCommand.h"

InputController::InputController(
    Player& p,
    Inventory& inv,
    WorldItemSystem& wis,
    GameController& g
)
    : player(p),
      inventory(inv),
      worldItemSystem(wis),
      game(g)
{
    inputHandler.bind(sf::Keyboard::Z, std::make_unique<MoveUpCommand>(player));
    inputHandler.bind(sf::Keyboard::Q, std::make_unique<MoveLeftCommand>(player));
    inputHandler.bind(sf::Keyboard::S, std::make_unique<MoveDownCommand>(player));
    inputHandler.bind(sf::Keyboard::D, std::make_unique<MoveRightCommand>(player));

    inputHandler.bind(
        sf::Mouse::Left,
        std::make_unique<AttackCommand>(player)
    );

    inputHandler.bind(sf::Mouse::Right, std::make_unique<UseItemCommand>(player));

    inputHandler.bind(
        sf::Keyboard::E,
        std::make_unique<PickupItemCommand>(
            player,
            worldItemSystem,
            game
        )
    );

    nextSlotCommand = std::make_unique<NextSlotCommand>(inventory, tabPressed);
    prevSlotCommand = std::make_unique<PrevSlotCommand>(inventory);

    // touches numériques
    inputHandler.bind(sf::Keyboard::Num1, std::make_unique<SelectSlotCommand>(inventory, 0));
    inputHandler.bind(sf::Keyboard::Num2, std::make_unique<SelectSlotCommand>(inventory, 1));
    inputHandler.bind(sf::Keyboard::Num3, std::make_unique<SelectSlotCommand>(inventory, 2));
    inputHandler.bind(sf::Keyboard::Num4, std::make_unique<SelectSlotCommand>(inventory, 3));
    inputHandler.bind(sf::Keyboard::Num5, std::make_unique<SelectSlotCommand>(inventory, 4));
    inputHandler.bind(sf::Keyboard::Num6, std::make_unique<SelectSlotCommand>(inventory, 5));
    inputHandler.bind(sf::Keyboard::Num7, std::make_unique<SelectSlotCommand>(inventory, 6));
    inputHandler.bind(sf::Keyboard::Num8, std::make_unique<SelectSlotCommand>(inventory, 7));
    inputHandler.bind(sf::Keyboard::Num9, std::make_unique<SelectSlotCommand>(inventory, 8));
}

void InputController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Tab)
    {
        inventory.setExpanded(true);
        tabPressed = true;
    }

    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Tab)
    {
        inventory.setExpanded(false);
        tabPressed = false;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            nextSlotCommand->execute(0.f);
        else
            prevSlotCommand->execute(0.f);
    }
}

void InputController::update(float dt) {
    inputHandler.handleInput(dt);
}
