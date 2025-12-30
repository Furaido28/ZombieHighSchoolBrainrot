#pragma once
#include <SFML/Graphics.hpp>
#include "models/headers/Player.h"

class PlayerView {
public:
    PlayerView();
    void render(sf::RenderWindow& window, const Player& player);

private:
    void playAnimation(
        sf::Texture& texture,
        int frameCount,
        int columns,
        float targetSize,
        bool animated
    );

private:
    sf::Sprite sprite;

    sf::Texture textureStatic;
    sf::Texture textureWalkLeft;
    sf::Texture textureWalkRight;
    sf::Texture textureWalkFront;
    sf::Texture textureWalkBack;

    sf::Clock animationTimer;
    int currentFrame = 0;
    float timePerFrame = 0.03f;

    // HUD - Vie
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;
    sf::RectangleShape hpOutline;
};
