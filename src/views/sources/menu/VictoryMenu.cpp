#include "views/headers/menu/VictoryMenu.h"

VictoryMenu::VictoryMenu(float width, float height) {
    font.loadFromFile("assets/fonts/font.ttf");

    // ===== TITLE =====
    title.setFont(font);
    title.setString("VICTORY");
    title.setCharacterSize(static_cast<unsigned>(height * 0.12f));
    title.setFillColor(sf::Color(120, 220, 120));

    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(width / 2.f, height * 0.30f);

    // ===== BACK =====
    backText.setFont(font);
    backText.setString("BACK TO MENU");
    backText.setCharacterSize(static_cast<unsigned>(height * 0.05f));
    backText.setFillColor(sf::Color::White);

    sf::FloatRect bb = backText.getLocalBounds();
    backText.setOrigin(bb.width / 2.f, bb.height / 2.f);
    backText.setPosition(width / 2.f, height * 0.55f);

    backRect.setFillColor(sf::Color::Transparent);
    backRect.setOutlineThickness(2.f);
    backRect.setOutlineColor(sf::Color::Transparent);

    updateRect();
}

void VictoryMenu::updateRect() {
    sf::FloatRect b = backText.getGlobalBounds();

    backRect.setSize({
        b.width + 60.f,
        b.height + 25.f
    });

    backRect.setOrigin(
        backRect.getSize().x / 2.f,
        backRect.getSize().y / 2.f
    );

    backRect.setPosition(
        b.left + b.width / 2.f,
        b.top  + b.height / 2.f
    );
}

bool VictoryMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    bool hover = backText.getGlobalBounds().contains(mouse);

    backText.setFillColor(
        hover ? sf::Color(255,230,120) : sf::Color::White
    );

    backRect.setOutlineColor(
        hover ? sf::Color(255,230,120,150) : sf::Color::Transparent
    );

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left &&
        hover)
        return true;

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter)
        return true;

    return false;
}

void VictoryMenu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(backRect);
    window.draw(backText);
}