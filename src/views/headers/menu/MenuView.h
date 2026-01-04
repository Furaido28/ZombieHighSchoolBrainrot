#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(float width, float height); // Constructor to create menu

    void draw(sf::RenderWindow& window); // Draw menu to window

    void moveUp(); // Move selection up
    void moveDown(); // Move selection down
    int getSelectedIndex() const; // Get selected item index

    // Check if mouse is on specific button
    bool isMouseOnItem(sf::Vector2f mousePos, int index) const;

    // Check if mouse is on any menu button
    bool isMouseOnAnyButton(sf::Vector2f mousePos) const;

    // Update hover effect based on mouse position
    void hoverWithMouse(sf::Vector2f mousePos);

private:
    std::vector<sf::Text> items; // Menu text items
    std::vector<sf::RectangleShape> buttonRects; // Button shapes
    sf::Font font; // Font for text

    sf::Texture backgroundTexture; // Menu background
    sf::Sprite backgroundSprite; // Background sprite

    int selectedIndex; // Currently selected item
};