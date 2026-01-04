#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Different categories of items
enum class ItemType {
    Consumable,
    Weapon,
    KeyFragment,
    LuckyBox
};

// Represents an item that can be picked up or used by the player
struct Item {
    // Display name of the item
    std::string name;

    // Item category (weapon, consumable, etc.)
    ItemType type;

    // Sprite used for rendering the item
    sf::Sprite sprite;

    // Generic value (heal amount, score, etc.)
    int value;

    // Indicates if the item can be picked up
    bool isPickable = true;

    // --- COMBAT STATS ---
    // Damage dealt by the item
    int damage = 0;

    // Attack range (melee hitbox size or max projectile distance)
    float range = 0.f;

    // Time between two attacks
    float cooldown = 0.f;

    // Projectile vs melee logic
    // true = projectile weapon, false = melee weapon
    bool isProjectile = false;

    // Speed of the projectile (if applicable)
    float projectileSpeed = 0.f;
};
