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
    int value;

    // --- NOUVELLES STATS DE COMBAT ---
    int damage = 0;             // Dégâts infligés
    float range = 0.f;          // Portée (taille hitbox melee ou distance max projectile)
    float cooldown = 0.f;       // Temps entre deux attaques

    // Logique Projectile vs Melee
    bool isProjectile = false;  // Si true = Craie/Gomme, si false = Règle/Poing
    float projectileSpeed = 0.f;// Vitesse si c'est un projectile
};