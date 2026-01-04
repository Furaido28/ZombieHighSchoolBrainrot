#pragma once
#include <SFML/Graphics.hpp>
#include "core/headers/Slider/Slider.h"
#include <vector>

class OptionsMenu {
public:
    OptionsMenu(float width, float height); // Constructor - initialize options menu

    void handleInput(sf::Keyboard::Key key); // Handle keyboard input
    void handleMouse(const sf::Event& event, sf::RenderWindow& window); // Handle mouse input

    void draw(sf::RenderWindow& window); // Draw the options menu
    int getSelectedIndex() const; // Get current selection

    // Check if back button was clicked
    bool buttonBackClicked(const sf::Event& event, sf::RenderWindow& window) const;

private:
    void updateLayout(); // Update positions and sizes

    float windowWidth; // Store window width
    float windowHeight; // Store window height

    sf::Font font; // Font for text
    std::vector<sf::Text> labels; // Text labels for options
    std::vector<Slider> sliders; // Slider controls

    sf::Text backText; // Back button text
    int selectedIndex; // Currently selected item
    int activeSlider; // Currently active slider

    int hoveredSlider; // Slider being hovered

    sf::RectangleShape backRect; // Back button shape

    sf::Texture backgroundTexture; // Menu background
    sf::Sprite backgroundSprite; // Background image

    // Check if mouse is over back button
    bool isMouseOnBack(sf::Vector2f mousePos) const;
};