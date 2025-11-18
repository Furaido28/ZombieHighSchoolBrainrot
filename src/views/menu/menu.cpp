#include "menu.h"
#include <iostream>

Menu::Menu(float width, float height) {
    selectedIndex = 0;

    if (!font.loadFromFile("assets/fonts/font.ttf")) {
        std::cerr << "Erreur : impossible de charger assets/fonts/font.ttf\n";
    }

    std::vector<std::string> labels = {
        "JOUER",
        "OPTIONS",
        "QUITTER"
    };

    const float spacing = 75.f;   // espace entre les boutons

    // 👉 Place le groupe de boutons à 100px du bas (change la valeur si tu veux)
    float bottomMargin = 100.f;

    // 👉 Calcule la hauteur totale du bloc (pour centrer verticalement le groupe)
    float totalHeight = spacing * (labels.size() - 1);

    // 👉 Position du premier bouton
    float baseY = height - bottomMargin - totalHeight;

    for (int i = 0; i < labels.size(); i++) {

        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(52);

        text.setFillColor(i == 0 ? sf::Color(255, 230, 120) : sf::Color::White);

        // centrer horizontalement
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f,
                       bounds.top  + bounds.height / 2.f);

        // 👉 Placer tout en bas
        text.setPosition(width / 2.f, baseY + i * spacing);

        items.push_back(text);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (const sf::Text& item : items)
        window.draw(item);
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        items[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        items[selectedIndex].setFillColor(sf::Color(255, 230, 120));
    }
}

void Menu::moveDown() {
    if (selectedIndex < items.size() - 1) {
        items[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        items[selectedIndex].setFillColor(sf::Color(255, 230, 120));
    }
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}

bool Menu::isMouseOnItem(sf::Vector2f mousePos, int index) const {
    return items[index].getGlobalBounds().contains(mousePos);
}

void Menu::hoverWithMouse(sf::Vector2f mousePos) {
    for (int i = 0; i < items.size(); i++) {
        if (items[i].getGlobalBounds().contains(mousePos)) {
            items[selectedIndex].setFillColor(sf::Color::White);
            selectedIndex = i;
            items[i].setFillColor(sf::Color(255, 230, 120));
            return;
        }
    }
}
