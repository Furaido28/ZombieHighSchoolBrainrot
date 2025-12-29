#include "../MenuController.h"

MenuController::MenuController(float width, float height)
: menu(width, height), choice(-1) {}

void MenuController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) menu.moveUp();
        if (event.key.code == sf::Keyboard::Down) menu.moveDown();
        if (event.key.code == sf::Keyboard::Enter)
            choice = menu.getSelectedIndex();
    }

    if (event.type == sf::Event::MouseMoved) {
        menu.hoverWithMouse({float(event.mouseMove.x), float(event.mouseMove.y)});
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f p(event.mouseButton.x, event.mouseButton.y);

        for (int i = 0; i < 3; i++)
            if (menu.isMouseOnItem(p, i))
                choice = i;
        }
}

void MenuController::update(float dt) {}

void MenuController::render(sf::RenderWindow& window) {
    menu.draw(window);
}

int MenuController::getSelectedChoice() const {
    return choice;
}
