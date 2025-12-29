#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ItemType {
    Consumable,
    Weapon,
    Quest,
    KeyFragment
};

struct Item {
    std::string name;
    ItemType type;
    sf::Sprite sprite;
};
