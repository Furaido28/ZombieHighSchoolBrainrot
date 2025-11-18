#include <SFML/Graphics.hpp>

int main() {
    // 🟦 Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Zombie High School Brainrot - Test SFML");
    window.setFramerateLimit(60);

    // 🟩 Un cercle pour tester l'affichage
    sf::CircleShape circle(80.f); // rayon = 80px
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(1280/2.f - 80.f, 720/2.f - 80.f);

    // 🔁 Boucle du jeu
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Fermeture de la fenêtre
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 🖥️ Effacer l'écran
        window.clear(sf::Color(30, 30, 30)); // gris foncé

        // 🟢 Dessiner le cercle
        window.draw(circle);

        // 📤 Afficher le rendu
        window.display();
    }

    return 0;
}