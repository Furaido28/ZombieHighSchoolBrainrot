#pragma once
#include <SFML/System.hpp>

class Player {
public:
    Player();

    void move(const sf::Vector2f& delta);

    void setPosition(float x, float y);
    void update(float dt);

    sf::Vector2f getPosition() const { return position; }

private:
    sf::Vector2f position;
    float speed;
};