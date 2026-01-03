#include "views/headers/menu/DefeatMenu.h"

DefeatMenu::DefeatMenu(float width, float height) {
    font.loadFromFile("assets/fonts/font.ttf");

    // ===== TITLE =====
    title.setFont(font);
    title.setString("DEFEAT");
    title.setCharacterSize(static_cast<unsigned>(height * 0.12f));
    title.setFillColor(sf::Color(200, 50, 50));

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
    backRect.setOutlineColor(sf::Color(255,230,120,150));

    updateRect();
}

void DefeatMenu::updateRect() {
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

bool DefeatMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {

    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    bool hover = backText.getGlobalBounds().contains(mouse);

    backText.setFillColor(hover ? sf::Color(255,230,120) : sf::Color::White);
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

void DefeatMenu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(backRect);
    window.draw(backText);
}

