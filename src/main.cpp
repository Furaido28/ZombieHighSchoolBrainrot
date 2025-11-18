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

    // --- CHARGEMENT DE L'ICÔNE ---
    sf::Image icon;

    // Chemins optimisés basés sur votre structure
    std::vector<std::string> possiblePaths = {
        "../assets/icon.png",        // Depuis cmake-build-debug/
        "../../assets/icon.png",     // Depuis la racine du projet
        "assets/icon.png",           // Depuis le dossier d'exécution
        "../src/assets/icon.png"     // Alternative
    };

    for (const auto& path : possiblePaths) {
        if (icon.loadFromFile(path)) {
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            break;
        }
    }

    Menu menu(WIDTH, HEIGHT);

    // Gestion du curseur
    bool cursorIsHand = false;
    sf::Cursor arrowCursor, handCursor;
    bool cursorsAvailable = arrowCursor.loadFromSystem(sf::Cursor::Arrow) &&
                           handCursor.loadFromSystem(sf::Cursor::Hand);

    if (cursorsAvailable) {
        window.setMouseCursor(arrowCursor);
    }

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Surlignage à la souris
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y)
                );
                menu.hoverWithMouse(mousePos);

                // Gestion du curseur
                if (cursorsAvailable) {
                    bool onButton = menu.isMouseOnAnyButton(mousePos);

                    if (onButton && !cursorIsHand) {
                        window.setMouseCursor(handCursor);
                        cursorIsHand = true;
                    } else if (!onButton && cursorIsHand) {
                        window.setMouseCursor(arrowCursor);
                        cursorIsHand = false;
                    }
                }
            }

            // Clic sur un item
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

            // Navigation clavier
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
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
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
            std::cout << "JOUER - Lancement du jeu...\n";
            // TODO: Implémenter le lancement du jeu
            break;

        case 1:
            std::cout << "OPTIONS - Ouverture des paramètres...\n";
            // TODO: Implémenter le menu options
            break;

        case 2:
            window.close();
            break;

        default:
            break;
    }
}