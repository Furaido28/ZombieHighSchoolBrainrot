#pragma once
#include <SFML/Graphics.hpp>

class VictoryMenu {
public:
    VictoryMenu(float width, float height);

    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void updateRect();

    sf::Font font;

    sf::Text title;
    sf::Text backText;
    sf::RectangleShape backRect;
};