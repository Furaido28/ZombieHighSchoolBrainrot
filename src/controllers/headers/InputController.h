#pragma once

/* ==========================================================
 * Input & Command System Includes
 * ========================================================== */
#include "core/headers/InputHandler.h"
#include "core/headers/commands/PrevSlotCommand.h"
#include "core/headers/commands/NextSlotCommand.h"

/* ==========================================================
 * Forward Declarations
 * ========================================================== */
class Player;
class Inventory;
class WorldItemSystem;
class GameController;

/* ==========================================================
 * InputController
 * Translates raw input events into game actions (Commands)
 * ========================================================== */
class InputController {
public:
    /* =========================
     * Constructor
     * ========================= */
    InputController(
        Player& player,
        Inventory& inventory,
        WorldItemSystem& worldItemSystem,
        GameController& game
    );

    /* =========================
     * Input Processing
     * ========================= */

    // Handles discrete SFML events (key pressed / released)
    void handleEvent(const sf::Event& event);

    // Handles continuous input (held keys, timing, etc.)
    void update(float dt);

private:
    /* ======================================================
     * Core Input Handler
     * ====================================================== */
    InputHandler inputHandler;

    /* ======================================================
     * References to Game Systems
     * ====================================================== */
    Player& player;
    Inventory& inventory;
    WorldItemSystem& worldItemSystem;
    GameController& game;

    /* ======================================================
     * Internal State
     * ====================================================== */
    bool tabPressed = false;  // Prevents repeated slot switching

    /* ======================================================
     * Command Pattern
     * ====================================================== */
    std::unique_ptr<NextSlotCommand> nextSlotCommand;
    std::unique_ptr<PrevSlotCommand> prevSlotCommand;
};
