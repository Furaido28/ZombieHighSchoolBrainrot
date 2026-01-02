#pragma once

#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include "Command.h"

enum class InputType {
    Keyboard,
    Mouse
};

struct InputBinding {
    InputType type;
    int code;
};

class InputHandler {
public:
    // Bind clavier
    void bind(sf::Keyboard::Key key, std::unique_ptr<Command> command) {
        bindings.emplace_back(
            InputBinding{ InputType::Keyboard, static_cast<int>(key) },
            std::move(command)
        );
    }

    // Bind souris
    void bind(sf::Mouse::Button button, std::unique_ptr<Command> command) {
        bindings.emplace_back(
            InputBinding{ InputType::Mouse, static_cast<int>(button) },
            std::move(command)
        );
    }

    void handleInput(float dt) {
        for (auto& [binding, command] : bindings) {
            bool pressed = false;

            if (binding.type == InputType::Keyboard) {
                pressed = sf::Keyboard::isKeyPressed(
                    static_cast<sf::Keyboard::Key>(binding.code));
            }
            else if (binding.type == InputType::Mouse) {
                pressed = sf::Mouse::isButtonPressed(
                    static_cast<sf::Mouse::Button>(binding.code));
            }

            if (pressed) {
                command->execute(dt);
            }
        }
    }

private:
    std::vector<std::pair<InputBinding, std::unique_ptr<Command>>> bindings;
};
