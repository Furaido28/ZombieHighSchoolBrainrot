#pragma once
#include <SFML/Graphics.hpp>
#include "../../../models/headers/Inventory.h"

class InventoryView {
public:
    InventoryView(Inventory& inventory);

    void update(bool tabPressed);
    void draw(sf::RenderWindow& window);

private:
    Inventory& inventory;
    bool showFull = false;

    sf::RectangleShape slotBg;

    void drawSlots(sf::RenderWindow& window, int count);
    void drawKeyFragments(sf::RenderWindow& window);
};
