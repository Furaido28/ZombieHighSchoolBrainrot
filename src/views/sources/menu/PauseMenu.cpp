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

    if (event.key.code == sf::Keyboard::Down &&
        selectedIndex < items.size() - 1) {

        items[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        items[selectedIndex].setFillColor(sf::Color(255,230,120));
        }
}


bool PauseMenu::handleMouse(const sf::Event& event, sf::RenderWindow& window) {

    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // 🔁 Hover
    bool hoverAny = false;

    for (int i = 0; i < items.size(); ++i) {
        if (isMouseOverItem(mousePos, i)) {
            if (selectedIndex != i) {
                items[selectedIndex].setFillColor(sf::Color::White);
                selectedIndex = i;
                items[i].setFillColor(sf::Color(255,230,120));
            }
            hoverAny = true;
        }
    }

    // 🧹 Souris hors menu
    if (!hoverAny) {
        for (int i = 0; i < items.size(); ++i) {
            items[i].setFillColor(
                i == selectedIndex ? sf::Color(255,230,120)
                                   : sf::Color::White
            );
        }
    }

    // 🖱️ Click
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {

        for (int i = 0; i < items.size(); ++i) {
            if (isMouseOverItem(mousePos, i)) {
                selectedIndex = i;
                return true;
            }
        }
    }

    return false;
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

bool PauseMenu::isMouseOverItem(sf::Vector2f mousePos, int index) const {
    return items[index].getGlobalBounds().contains(mousePos);
}
