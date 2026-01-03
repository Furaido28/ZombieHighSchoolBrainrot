#pragma once
#include <SFML/Graphics.hpp>

class DefeatMenu {
public:
    DefeatMenu(float width, float height);

    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text title;
    sf::Text backText;
    sf::RectangleShape backRect;

    void updateRect();
};
