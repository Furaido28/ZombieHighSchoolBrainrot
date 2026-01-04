/* ==========================================================
 * Includes
 * ========================================================== */
#include "controllers/headers/InputController.h"

#include "core/headers/commands/MoveUpCommand.h"
#include "core/headers/commands/MoveDownCommand.h"
#include "core/headers/commands/MoveLeftCommand.h"
#include "core/headers/commands/MoveRightCommand.h"
#include "core/headers/commands/AttackCommand.h"
#include "core/headers/commands/PickupItemCommand.h"
#include "core/headers/commands/SelectSlotCommand.h"
#include "core/headers/commands/UseItemCommand.h"

/* ==========================================================
 * InputController Constructor
 * Initializes all input bindings (keyboard & mouse)
 * ========================================================== */
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
    /* =========================
     * Movement Commands
     * ========================= */

    // ZQSD movement bindings
    inputHandler.bind(sf::Keyboard::Z, std::make_unique<MoveUpCommand>(player));
    inputHandler.bind(sf::Keyboard::Q, std::make_unique<MoveLeftCommand>(player));
    inputHandler.bind(sf::Keyboard::S, std::make_unique<MoveDownCommand>(player));
    inputHandler.bind(sf::Keyboard::D, std::make_unique<MoveRightCommand>(player));

    /* =========================
     * Combat & Item Usage
     * ========================= */

    // Left mouse button: attack
    inputHandler.bind(
        sf::Mouse::Left,
        std::make_unique<AttackCommand>(player)
    );

    // Right mouse button: use selected item
    inputHandler.bind(sf::Mouse::Right, std::make_unique<UseItemCommand>(player));

    /* =========================
     * Item Pickup
     * ========================= */

    // E key: pick up item from the world
    inputHandler.bind(
        sf::Keyboard::E,
        std::make_unique<PickupItemCommand>(
            player,
            worldItemSystem,
            game
        )
    );

    /* =========================
     * Inventory Slot Navigation
     * ========================= */

    // Mouse wheel slot navigation commands
    nextSlotCommand = std::make_unique<NextSlotCommand>(inventory, tabPressed);
    prevSlotCommand = std::make_unique<PrevSlotCommand>(inventory);

    /* =========================
     * Numeric Slot Selection
     * ========================= */

    // Numeric keys to directly select inventory slots
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

/* ==========================================================
 * InputController::handleEvent
 * Handles discrete SFML events
 * ========================================================== */
void InputController::handleEvent(const sf::Event& event) {

    /* =========================
     * Inventory Expansion (TAB)
     * ========================= */

    // TAB pressed: expand inventory
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Tab)
    {
        inventory.setExpanded(true);
        tabPressed = true;
    }

    // TAB released: collapse inventory
    if (event.type == sf::Event::KeyReleased &&
        event.key.code == sf::Keyboard::Tab)
    {
        inventory.setExpanded(false);
        tabPressed = false;
    }

    /* =========================
     * Mouse Wheel Slot Selection
     * ========================= */

    // Mouse wheel scroll: switch inventory slot
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            nextSlotCommand->execute(0.f);
        else
            prevSlotCommand->execute(0.f);
    }
}

/* ==========================================================
 * InputController::update
 * Processes continuous input actions
 * ========================================================== */
void InputController::update(float dt) {
    // Execute commands bound to continuous inputs
    inputHandler.handleInput(dt);
}