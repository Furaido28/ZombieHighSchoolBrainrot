#pragma once
#include <SFML/Graphics.hpp>
#include "../../../models/headers/Player.h"

class PlayerView {
public:
    PlayerView();
    void render(sf::RenderWindow& window, const Player& player);

private:
    // Texture pour l'immobile (votre petite image)
    sf::Texture textureStatic;

    // Texture pour la marche gauche (votre grosse image)
    sf::Texture textureWalkLeft;

    sf::Sprite sprite;

    // Pour l'animation
    sf::Clock animationTimer;
    int currentFrame;
    float timePerFrame;
};