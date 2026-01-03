#include "views/headers/menu/PauseMenu.h"

PauseMenu::PauseMenu(float width, float height)
    : selectedIndex(0) {

    font.loadFromFile("assets/fonts/font.ttf");

    std::vector<std::string> labels = {
        "RESUME",
        "OPTIONS",
        "QUIT TO MENU"
    };

    float startY = height * 0.45f;
    float spacing = height * 0.08f;

    for (int i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(static_cast<unsigned int>(height * 0.05f));
        text.setFillColor(i == 0 ? sf::Color(255,230,120) : sf::Color::White);

        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        text.setPosition(width / 2.f, startY + i * spacing);

        items.push_back(text);
    }
}

void PauseMenu::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::Up && selectedIndex > 0) {
        items[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        items[selectedIndex].setFillColor(sf::Color(255,230,120));
    }

    if (event.key.code == sf::Keyboard::Down && selectedIndex < items.size() - 1) {
        items[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        items[selectedIndex].setFillColor(sf::Color(255,230,120));
    }
}

void PauseMenu::draw(sf::RenderWindow& window) {
    for (auto& item : items)
        window.draw(item);
}

int PauseMenu::getSelectedIndex() const {
    return selectedIndex;
}

void PauseMenu::reset() {
    selectedIndex = 0;

    for (int i = 0; i < items.size(); ++i) {
        items[i].setFillColor(
            i == 0 ? sf::Color(255,230,120) : sf::Color::White
        );
    }
}

