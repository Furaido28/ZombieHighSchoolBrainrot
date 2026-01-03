#include "core/headers/Slider/Slider.h"
#include <algorithm>

Slider::Slider()
    : value(50.f) {

    barBackground.setFillColor(sf::Color(120, 120, 120));
    barFill.setFillColor(sf::Color(255, 230, 120));
}

void Slider::setLayout(float x, float y, float width, float height) {
    barBackground.setSize({width, height});
    barBackground.setPosition(x, y);

    barFill.setSize({width * (value / 100.f), height});
    barFill.setPosition(x, y);
}

void Slider::setValue(float v) {
    value = std::clamp(v, 0.f, 100.f);

    float width = barBackground.getSize().x;
    float height = barBackground.getSize().y;

    barFill.setSize({width * (value / 100.f), height});
}

float Slider::getValue() const {
    return value;
}

void Slider::increase(float step) {
    setValue(value + step);
}

void Slider::decrease(float step) {
    setValue(value - step);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(barBackground);
    window.draw(barFill);
}

bool Slider::contains(sf::Vector2f mousePos) const {
    return barBackground.getGlobalBounds().contains(mousePos);
}

void Slider::updateFromMouse(sf::Vector2f mousePos) {
    float x = barBackground.getPosition().x;
    float width = barBackground.getSize().x;

    float clampedX = std::clamp(mousePos.x, x, x + width);
    float percent = (clampedX - x) / width * 100.f;

    setValue(percent);
}
