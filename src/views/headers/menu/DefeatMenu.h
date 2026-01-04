#pragma once
#include <SFML/Graphics.hpp>

class DefeatMenu {
public:
    // Create the defeat menu with screen size
    DefeatMenu(float width, float height);

    // Handle mouse/keyboard events
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Draw the defeat menu
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text title;     // "Defeat" title
    sf::Text backText;  // Back to menu button
    sf::RectangleShape backRect; // Button background

    // Update button position and size
    void updateRect();
};
