#pragma once

#include <SFML/Window.hpp>
#include <map>
#include <memory>

#include "Command.h"

class InputHandler {
public:
    void bind(sf::Keyboard::Key key, std::unique_ptr<Command> command) {
        commands[key] = std::move(command);
    }

    void handleInput(float dt) {
        for (auto& [key, command] : commands) {
            if (sf::Keyboard::isKeyPressed(key)) {
                command->execute(dt);
            }
        }
    }

private:
    std::map<sf::Keyboard::Key, std::unique_ptr<Command>> commands;

};
