#include "core/headers/Slider/Slider.h"
#include <algorithm>

// Constructor: initializes default slider value and colors
Slider::Slider()
    : value(50.f) {

    // Background bar color
    barBackground.setFillColor(sf::Color(120, 120, 120));

    // Filled part color (current value)
    barFill.setFillColor(sf::Color(255, 230, 120));
}

// Sets the position and size of the slider
void Slider::setLayout(float x, float y, float width, float height) {
    // Background bar
    barBackground.setSize({width, height});
    barBackground.setPosition(x, y);

    // Filled bar based on current value
    barFill.setSize({width * (value / 100.f), height});
    barFill.setPosition(x, y);
}

// Sets the slider value (clamped between 0 and 100)
void Slider::setValue(float v) {
    value = std::clamp(v, 0.f, 100.f);

    float width = barBackground.getSize().x;
    float height = barBackground.getSize().y;

    // Update filled bar size according to value
    barFill.setSize({width * (value / 100.f), height});
}

// Returns current slider value
float Slider::getValue() const {
    return value;
}

// Increases the value by a given step
void Slider::increase(float step) {
    setValue(value + step);
}

// Decreases the value by a given step
void Slider::decrease(float step) {
    setValue(value - step);
}

// Draws the slider on screen
void Slider::draw(sf::RenderWindow& window) {
    window.draw(barBackground);
    window.draw(barFill);
}

// Checks if the mouse is over the slider
bool Slider::contains(sf::Vector2f mousePos) const {
    return barBackground.getGlobalBounds().contains(mousePos);
}

// Updates the slider value based on mouse position
void Slider::updateFromMouse(sf::Vector2f mousePos) {
    float x = barBackground.getPosition().x;
    float width = barBackground.getSize().x;

    // Clamp mouse position inside the slider
    float clampedX = std::clamp(mousePos.x, x, x + width);

    // Convert mouse position to percentage
    float percent = (clampedX - x) / width * 100.f;

    setValue(percent);
}