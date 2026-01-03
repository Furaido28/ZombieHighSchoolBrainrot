#pragma once
#include <SFML/Graphics.hpp>

class Slider {
public:
    Slider();

    void setLayout(float x, float y, float width, float height);

    void setValue(float v);
    float getValue() const;

    void increase(float step);
    void decrease(float step);

    // 🖱️ Souris
    bool contains(sf::Vector2f mousePos) const;
    void updateFromMouse(sf::Vector2f mousePos);

    void draw(sf::RenderWindow& window);

private:
    float value;
    sf::RectangleShape barBackground;
    sf::RectangleShape barFill;
};

