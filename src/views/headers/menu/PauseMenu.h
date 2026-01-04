#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PauseMenu {
public:
    PauseMenu(float width, float height); // Constructor

    void handleEvent(const sf::Event& event);   // Handle events
    bool handleMouse(const sf::Event& event, sf::RenderWindow& window); // Handle mouse input

    void draw(sf::RenderWindow& window); // Draw pause menu

    int getSelectedIndex() const; // Get selected menu item
    void reset(); // Reset menu state

private:
    sf::Font font; // Text font
    std::vector<sf::Text> items; // Menu options
    int selectedIndex; // Currently selected item

    sf::RectangleShape selectionRect; // Visual selection indicator
    void updateSelectionRect(); // Update selection rectangle position

    // Check if mouse is over specific menu item
    bool isMouseOverItem(sf::Vector2f mousePos, int index) const;
};