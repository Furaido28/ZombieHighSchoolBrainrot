#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Inventory.h"

// 1. On définit les directions (L'ordre est important pour ton sprite sheet)
enum class Direction { Down = 0, Left = 1, Right = 2, Up = 3 };

class Player {
public:
    Player();

    void move(const sf::Vector2f& delta);
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void update(float dt);
    float getRadius() const;

    // 2. Méthodes pour gérer la direction
    void setDirection(Direction dir);
    Direction getDirection() const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getGlobalBounds() const;

    void setMoving(bool moving);
    bool isMoving() const;
    Inventory& getInventory();

private:
    sf::Vector2f position;
    sf::Vector2f size;
    float speed;
    bool moving;
    Inventory inventory;

    // 3. Variable pour stocker l'état actuel
    Direction currentDirection;
};