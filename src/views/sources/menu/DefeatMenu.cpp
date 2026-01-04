#include "views/headers/menu/DefeatMenu.h"

// Constructor: initializes the defeat screen elements
DefeatMenu::DefeatMenu(float width, float height) {
    // Load font
    font.loadFromFile("assets/fonts/font.ttf");

    // ===== TITLE =====
    // Configure the "DEFEAT" title
    title.setFont(font);
    title.setString("DEFEAT");
    title.setCharacterSize(static_cast<unsigned>(height * 0.12f));
    title.setFillColor(sf::Color(200, 50, 50));

    // Center the title
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(width / 2.f, height * 0.30f);

    // ===== BACK BUTTON =====
    // Configure "BACK TO MENU" text
    backText.setFont(font);
    backText.setString("BACK TO MENU");
    backText.setCharacterSize(static_cast<unsigned>(height * 0.05f));
    backText.setFillColor(sf::Color::White);

    // Center the back text
    sf::FloatRect bb = backText.getLocalBounds();
    backText.setOrigin(bb.width / 2.f, bb.height / 2.f);
    backText.setPosition(width / 2.f, height * 0.55f);

    // Configure back button outline
    backRect.setFillColor(sf::Color::Transparent);
    backRect.setOutlineThickness(2.f);
    backRect.setOutlineColor(sf::Color(255,230,120,150));

    // Adjust rectangle size to text
    updateRect();
}

// Updates the size and position of the back button rectangle
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

// Handles mouse and keyboard events
// Returns true when the user wants to go back to menu
bool DefeatMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {

    // Get mouse position in world coordinates
    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Check hover on back text
    bool hover = backText.getGlobalBounds().contains(mouse);

    // Visual feedback on hover
    backText.setFillColor(hover ? sf::Color(255,230,120) : sf::Color::White);
    backRect.setOutlineColor(
        hover ? sf::Color(255,230,120,150) : sf::Color::Transparent
    );

    // Mouse click validation
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left &&
        hover)
        return true;

    // Keyboard validation (Enter key)
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter)
        return true;

    return false;
}

// Draws the defeat menu
void DefeatMenu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(backRect);
    window.draw(backText);
}
