#include <SFML/Graphics.hpp>
#include <iostream>
#include "views/menu/menu.h"

const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 720;

// Déclaration de la fonction
void handleSelection(int choice, sf::RenderWindow& window);

int main() {

    sf::RenderWindow window(
        sf::VideoMode(WIDTH, HEIGHT),
        "Zombie High School Brainrot - Menu"
    );
    window.setFramerateLimit(60);

    // --- Menu principal ---
    Menu menu(WIDTH, HEIGHT);

    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {

            // --- Fermeture fenêtre ---
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // --- Surlignage à la souris ---
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y)
                );
                menu.hoverWithMouse(mousePos);
            }

            // --- Clic sur un item ---
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

            // --- Navigation clavier ---
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

        // --- AFFICHAGE ---
        window.clear(sf::Color(30, 30, 30));  // fond gris foncé
        menu.draw(window);
        window.display();
    }

    return 0;
}

// Fonction de gestion des sélections
void handleSelection(int choice, sf::RenderWindow& window) {
    switch (choice) {
        case 0:
            std::cout << "JOUER\n";
            // TODO: lancer le jeu
            break;

        case 1:
            std::cout << "OPTIONS\n";
            // TODO: menu options
            break;

        case 2:
            window.close();
            break;

        default:
            break;
    }
}