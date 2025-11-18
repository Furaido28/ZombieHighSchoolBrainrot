#include <SFML/Graphics.hpp>
#include <iostream>
#include "views/menu/menu.h"

const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 720;

void handleSelection(int choice, sf::RenderWindow& window);

int main() {
    sf::RenderWindow window(
        sf::VideoMode(WIDTH, HEIGHT),
        "Zombie High School Brainrot - Menu"
    );
    window.setFramerateLimit(60);

    Menu menu(WIDTH, HEIGHT);

    // Variables pour gérer le curseur
    bool cursorIsHand = false;

    // Initialiser le curseur une fois au début
    sf::Cursor cursor;
    if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
        window.setMouseCursor(cursor);
    }

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y)
                );
                menu.hoverWithMouse(mousePos);

                // Vérifier si on doit changer le curseur
                bool onButton = menu.isMouseOnAnyButton(mousePos);

                if (onButton && !cursorIsHand) {
                    // Changer en main
                    sf::Cursor handCursor;
                    if (handCursor.loadFromSystem(sf::Cursor::Hand)) {
                        window.setMouseCursor(handCursor);
                        cursorIsHand = true;
                    }
                } else if (!onButton && cursorIsHand) {
                    // Changer en flèche
                    sf::Cursor arrowCursor;
                    if (arrowCursor.loadFromSystem(sf::Cursor::Arrow)) {
                        window.setMouseCursor(arrowCursor);
                        cursorIsHand = false;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                for (int i = 0; i < 3; i++) {
                    if (menu.isMouseOnItem(mousePos, i)) {
                        handleSelection(i, window);
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    int choice = menu.getSelectedIndex();
                    handleSelection(choice, window);
                }
            }
        }

        // Affichage
        window.clear(sf::Color(30, 30, 30));
        menu.draw(window);
        window.display();
    }

    return 0;
}

void handleSelection(int choice, sf::RenderWindow& window) {
    switch (choice) {
        case 0:
            std::cout << "JOUER\n";
            break;
        case 1:
            std::cout << "OPTIONS\n";
            break;
        case 2:
            window.close();
            break;
    }
}