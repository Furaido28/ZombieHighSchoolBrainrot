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
    int currentFrame;
    float timePerFrame;
};