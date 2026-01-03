#pragma once
#include <SFML/Graphics.hpp>
#include "core/headers/Slider/Slider.h"
#include <vector>

class OptionsMenu {
public:
    OptionsMenu(float width, float height);

    void handleInput(sf::Keyboard::Key key);
    void handleMouse(const sf::Event& event, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
    int getSelectedIndex() const;

    bool buttonBackClicked(const sf::Event& event, sf::RenderWindow& window) const;
private:
    void updateLayout();

    float windowWidth;
    float windowHeight;

    sf::Font font;
    std::vector<sf::Text> labels;
    std::vector<Slider> sliders;

    sf::Text backText;
    int selectedIndex;
    int activeSlider;

    int hoveredSlider;

    sf::RectangleShape backRect;

    bool isMouseOnBack(sf::Vector2f mousePos) const;

};

