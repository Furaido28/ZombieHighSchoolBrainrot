#pragma once

#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include "Command.h"

// Defines if the input comes from Keyboard or Mouse
enum class InputType {
    Keyboard,
    Mouse
};

// Helper struct to store the key/button code
struct InputBinding {
    InputType type;
    int code;
};

// Manages user inputs and maps them to game commands
class InputHandler {
public:
    // Bind a keyboard key to a command (e.g., 'W' -> MoveUp)
    void bind(sf::Keyboard::Key key, std::unique_ptr<Command> command) {
        bindings.emplace_back(
            InputBinding{ InputType::Keyboard, static_cast<int>(key) },
            std::move(command)
        );
    }

    // Bind a mouse button to a command
    void bind(sf::Mouse::Button button, std::unique_ptr<Command> command) {
        bindings.emplace_back(
            InputBinding{ InputType::Mouse, static_cast<int>(button) },
            std::move(command)
        );
    }

    // Check all inputs every frame and execute commands if keys are pressed
    void handleInput(float dt) {
        for (auto& [binding, command] : bindings) {
            bool pressed = false;

            // Check if keyboard key is held down
            if (binding.type == InputType::Keyboard) {
                pressed = sf::Keyboard::isKeyPressed(
                    static_cast<sf::Keyboard::Key>(binding.code));
            }
            // Check if mouse button is held down
            else if (binding.type == InputType::Mouse) {
                pressed = sf::Mouse::isButtonPressed(
                    static_cast<sf::Mouse::Button>(binding.code));
            }

            // If the key/button is active, run the command
            if (pressed) {
                command->execute(dt);
            }
        }
    }

private:
    // List of all active key/button bindings
    std::vector<std::pair<InputBinding, std::unique_ptr<Command>>> bindings;
};