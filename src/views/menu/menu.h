#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(float width, float height);

    void draw(sf::RenderWindow& window);

    void moveUp();
    void moveDown();
    int getSelectedIndex() const;

    bool isMouseOnItem(sf::Vector2f mousePos, int index) const;
    bool isMouseOnAnyButton(sf::Vector2f mousePos) const;
    void hoverWithMouse(sf::Vector2f mousePos);

private:
    std::vector<sf::Text> items;
    std::vector<sf::RectangleShape> buttonRects;
    sf::Font font;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    int selectedIndex;
};
