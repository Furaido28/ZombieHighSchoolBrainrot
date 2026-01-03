#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PauseMenu {
public:
    PauseMenu(float width, float height);

    void handleEvent(const sf::Event& event);   // 🔑 OBLIGATOIRE
    bool handleMouse(const sf::Event& event, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    int getSelectedIndex() const;
    void reset();

private:
    sf::Font font;
    std::vector<sf::Text> items;
    int selectedIndex;

    sf::RectangleShape selectionRect;
    void updateSelectionRect();

    bool isMouseOverItem(sf::Vector2f mousePos, int index) const;
};
