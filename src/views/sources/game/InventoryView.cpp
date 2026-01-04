#include "./models/headers/Inventory.h"
#include "views/headers/game/InventoryView.h"

InventoryView::InventoryView(Inventory& inv)
    : inventory(inv)
{
    // Initialize slot background
    slotBg.setSize({64.f, 64.f});
    slotBg.setFillColor(sf::Color(0, 0, 0, 160));
    slotBg.setOutlineThickness(2);
    slotBg.setOutlineColor(sf::Color::White);
}

void InventoryView::update(bool tabPressed) {
    showFull = tabPressed; // Show full inventory when Tab is pressed
}

void InventoryView::draw(sf::RenderWindow& window) {
    drawSlots(window, showFull ? 9 : 3); // Draw 9 slots if full, 3 if not
    drawKeyFragments(window); // Draw key fragments
}

void InventoryView::drawSlots(sf::RenderWindow& window, int count) {
    const auto& slots = inventory.getSlots();
    int selected = inventory.getSelectedSlot();

    for (int i = 0; i < count; i++) {
        // Highlight selected slot
        if (i == selected) {
            slotBg.setOutlineColor(sf::Color::Yellow);
            slotBg.setOutlineThickness(3);
        } else {
            slotBg.setOutlineColor(sf::Color::White);
            slotBg.setOutlineThickness(2);
        }

        // Position slot
        slotBg.setPosition(20 + i * 70, window.getSize().y - 90);
        window.draw(slotBg);

        // Draw item if slot has one
        if (slots[i].has_value()) {
            auto sprite = slots[i]->sprite;
            sprite.setPosition(slotBg.getPosition());
            window.draw(sprite);
        }
    }
}

void InventoryView::drawKeyFragments(sf::RenderWindow& window) {
    const auto& fragments = inventory.getKeyFragments();

    for (int i = 0; i < fragments.size(); i++) {
        auto sprite = fragments[i].sprite;
        sprite.setPosition(20 + i * 50, window.getSize().y - 160);
        window.draw(sprite);
    }
}