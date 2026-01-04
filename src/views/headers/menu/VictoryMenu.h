#pragma once
#include <SFML/Graphics.hpp>

class VictoryMenu {
public:
    VictoryMenu(float width, float height); // Constructor

    // Handle user input events
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Draw victory menu
    void draw(sf::RenderWindow& window);

private:
    void updateRect(); // Update button position/size

    sf::Font font; // Text font

    sf::Text title; // Victory title text
    sf::Text backText; // Back button text
    sf::RectangleShape backRect; // Back button shape
};