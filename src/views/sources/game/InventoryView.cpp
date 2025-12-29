#include "./models/headers/Inventory.h"
#include "views/headers/game/InventoryView.h"

InventoryView::InventoryView(Inventory& inv)
    : inventory(inv)
{
    slotBg.setSize({64.f, 64.f});
    slotBg.setFillColor(sf::Color(0, 0, 0, 160));
    slotBg.setOutlineThickness(2);
    slotBg.setOutlineColor(sf::Color::White);
}

void InventoryView::update(bool tabPressed) {
    showFull = tabPressed;
}

void InventoryView::draw(sf::RenderWindow& window) {
    drawSlots(window, showFull ? 9 : 3);
    drawKeyFragments(window);
}

void InventoryView::drawSlots(sf::RenderWindow& window, int count) {
    const auto& slots = inventory.getSlots();
    int selected = inventory.getSelectedSlot();

    for (int i = 0; i < count; i++) {

        // 🔥 Highlight slot sélectionné
        if (i == selected) {
            slotBg.setOutlineColor(sf::Color::Yellow);
            slotBg.setOutlineThickness(3);
        } else {
            slotBg.setOutlineColor(sf::Color::White);
            slotBg.setOutlineThickness(2);
        }

        slotBg.setPosition(20 + i * 70, window.getSize().y - 90);
        window.draw(slotBg);

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
