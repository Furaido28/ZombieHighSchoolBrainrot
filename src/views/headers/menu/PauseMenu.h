#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PauseMenu {
public:
    PauseMenu(float width, float height);

    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    int getSelectedIndex() const;
    void reset();

private:
    sf::Font font;
    std::vector<sf::Text> items;
    int selectedIndex;
};
