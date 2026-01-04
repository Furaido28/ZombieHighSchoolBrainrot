#include "../../headers/menu/MenuView.h"
#include <iostream>

// Constructor: initializes the main menu (background, font, buttons)
Menu::Menu(float width, float height) {
    selectedIndex = 0;

    // Load background image
    if (!backgroundTexture.loadFromFile("assets/menus/title_screen.png")) {
        std::cerr << "Erreur : impossible de charger assets/menus/title_screen.png\n";
    } else {
        backgroundSprite.setTexture(backgroundTexture);

        // Scale background to fit the window
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2f scaleFactors(
            width / static_cast<float>(textureSize.x),
            height / static_cast<float>(textureSize.y)
        );
        backgroundSprite.setScale(scaleFactors);
    }

    // Load font
    if (!font.loadFromFile("assets/fonts/font.ttf")) {
        std::cerr << "Erreur : impossible de charger assets/fonts/font.ttf\n";
    }

    // Menu item labels
    std::vector<std::string> labels = {
        "PLAY",
        "OPTIONS",
        "EXIT"
    };

    // Layout configuration
    const float spacing = 75.f;
    const float bottomMargin = 100.f;
    const float totalHeight = spacing * (labels.size() - 1);
    const float baseY = height - bottomMargin - totalHeight;

    const float rectWidth = 300.f;
    const float rectHeight = 60.f;
    const float outlineThickness = 2.f;

    // Create menu items and their selection rectangles
    for (int i = 0; i < labels.size(); i++) {

        // Create text item
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(52);
        text.setFillColor(i == 0 ? sf::Color(255, 230, 120) : sf::Color::White);

        // Center text origin
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f,
                       bounds.top + bounds.height / 2.f);
        text.setPosition(width / 2.f, baseY + i * spacing);

        // Create selection rectangle
        sf::RectangleShape buttonRect;
        buttonRect.setSize(sf::Vector2f(rectWidth, rectHeight));
        buttonRect.setOrigin(rectWidth / 2.f, rectHeight / 2.f);
        buttonRect.setPosition(width / 2.f, baseY + i * spacing);

        buttonRect.setFillColor(sf::Color::Transparent);
        buttonRect.setOutlineColor(sf::Color(255, 230, 120, 150));
        buttonRect.setOutlineThickness(outlineThickness);

        // Only first item is highlighted at start
        if (i != 0) {
            buttonRect.setOutlineColor(sf::Color::Transparent);
        }

        items.push_back(text);
        buttonRects.push_back(buttonRect);
    }
}

// Draws the menu on screen
void Menu::draw(sf::RenderWindow& window) {
    if (backgroundTexture.getSize().x > 0) {
        window.draw(backgroundSprite);
    }

    // Draw button outlines
    for (const sf::RectangleShape& rect : buttonRects) {
        window.draw(rect);
    }

    // Draw menu texts
    for (const sf::Text& item : items) {
        window.draw(item);
    }
}

// Move selection up using keyboard
void Menu::moveUp() {
    if (selectedIndex > 0) {
        items[selectedIndex].setFillColor(sf::Color::White);
        buttonRects[selectedIndex].setOutlineColor(sf::Color::Transparent);

        selectedIndex--;
        items[selectedIndex].setFillColor(sf::Color(255, 230, 120));
        buttonRects[selectedIndex].setOutlineColor(sf::Color(255, 230, 120, 150));
    }
}

// Move selection down using keyboard
void Menu::moveDown() {
    if (selectedIndex < items.size() - 1) {
        items[selectedIndex].setFillColor(sf::Color::White);
        buttonRects[selectedIndex].setOutlineColor(sf::Color::Transparent);

        selectedIndex++;
        items[selectedIndex].setFillColor(sf::Color(255, 230, 120));
        buttonRects[selectedIndex].setOutlineColor(sf::Color(255, 230, 120, 150));
    }
}

// Returns the currently selected menu index
int Menu::getSelectedIndex() const {
    return selectedIndex;
}

// Checks if mouse is over a specific menu item
bool Menu::isMouseOnItem(sf::Vector2f mousePos, int index) const {
    return buttonRects[index].getGlobalBounds().contains(mousePos);
}

// Checks if mouse is over any menu button
bool Menu::isMouseOnAnyButton(sf::Vector2f mousePos) const {
    for (const auto& rect : buttonRects) {
        if (rect.getGlobalBounds().contains(mousePos)) {
            return true;
        }
    }
    return false;
}

// Updates selection when hovering with mouse
void Menu::hoverWithMouse(sf::Vector2f mousePos) {
    bool foundHover = false;

    for (int i = 0; i < items.size(); i++) {
        if (buttonRects[i].getGlobalBounds().contains(mousePos)) {
            if (selectedIndex != i) {
                items[selectedIndex].setFillColor(sf::Color::White);
                buttonRects[selectedIndex].setOutlineColor(sf::Color::Transparent);

                selectedIndex = i;
                items[i].setFillColor(sf::Color(255, 230, 120));
                buttonRects[i].setOutlineColor(sf::Color(255, 230, 120, 150));
            }
            foundHover = true;
            break;
        }
    }

    // Remove outline if mouse is not hovering any button
    if (!foundHover && selectedIndex >= 0 && selectedIndex < items.size()) {
        buttonRects[selectedIndex].setOutlineColor(sf::Color::Transparent);
    }
}