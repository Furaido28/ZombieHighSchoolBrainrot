#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Inventory.h"
#include "Item.h" // Nécessaire pour connaître les types d'armes

// 1. On définit les directions
enum class Direction { Down = 0, Left = 1, Right = 2, Up = 3 };

// --- NOUVEAU : Structure pour renvoyer les données de l'attaque au Controller ---
struct AttackInfo {
    bool valid = false;         // L'attaque a-t-elle lieu ? (false si cooldown ou pas d'arme)
    bool isProjectile = false;  // true = tir (craie), false = corps à corps (règle)
    int damage = 0;

    // Si Melee (Zone de dégâts rectangulaire devant le joueur)
    sf::FloatRect meleeHitbox;

    // Si Projectile (Position de départ et vitesse)
    sf::Vector2f startPosition;
    sf::Vector2f velocity;
};

class Player {
public:
    Player();

    // Stats de vie
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int value);
    void takeDamage(int amount);
    bool isAlive() const;
    bool isInvincible() const;

    // Mouvement et Position
    void move(const sf::Vector2f& delta);
    void setPosition(float x, float y);
    void setSize(float w, float h);

    // Update gère maintenant les timers d'invincibilité ET de cooldown d'attaque
    void update(float dt);

    float getRadius() const;

    // Gestion de la direction
    void setDirection(Direction dir);
    Direction getDirection() const;

    // Hitbox et Taille
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getGlobalBounds() const;

    // États
    void setMoving(bool moving);
    bool isMoving() const;
    Inventory& getInventory();

    // --- NOUVEAU : Méthode pour déclencher une attaque ---
    AttackInfo tryAttack();

private:
    int health;
    int maxHealth;
    float invincibilityTimer = 0.f;

    // --- NOUVEAU : Timer pour empêcher de spammer les attaques ---
    float attackCooldownTimer = 0.f;

    sf::Vector2f position;
    sf::Vector2f size;
    float speed;
    bool moving;
    Inventory inventory;

    // Direction actuelle (pour savoir où taper)
    Direction currentDirection;
};