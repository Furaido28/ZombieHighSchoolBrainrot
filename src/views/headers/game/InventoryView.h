#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Inventory.h"

class InventoryView {
public:
    // Constructor: links the view to the inventory model
    InventoryView(Inventory& inventory);

    // Handle inventory visibility (TAB key)
    void update(bool tabPressed);

    // Draw the inventory on screen
    void draw(sf::RenderWindow& window);

private:
    Inventory& inventory;
    bool showFull = false; // Show full inventory or compact view

    sf::RectangleShape slotBg; // Background of one slot

    // Draw inventory slots
    void drawSlots(sf::RenderWindow& window, int count);

    // Draw collected key fragments
    void drawKeyFragments(sf::RenderWindow& window);
};