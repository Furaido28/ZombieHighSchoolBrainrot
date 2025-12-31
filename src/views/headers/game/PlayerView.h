#pragma once
#include <SFML/Graphics.hpp>
#include "models/headers/Player.h"

class PlayerView {
public:
    PlayerView();

    // Dessin du joueur dans le monde (avec caméra)
    void renderWorld(sf::RenderWindow& window, const Player& player);

    // Dessin du HUD (sans caméra)
    void renderHUD(sf::RenderWindow& window, const Player& player);

private:
    void playAnimation(
        sf::Texture& texture,
        int frameCount,
        int columns,
        float targetSize
    );

    sf::Sprite sprite;

    // Textures
    sf::Texture textureIdle;
    sf::Texture textureLeft;
    sf::Texture textureRight;
    sf::Texture textureUp;
    sf::Texture textureDown;

    // Animation
    sf::Clock animationClock;
    int currentFrame = 0;
    float frameTime = 0.03f;

    // HUD
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;
    sf::RectangleShape hpOutline;

    sf::Text hpText;
    sf::Font hudFont;

    sf::Texture heartTexture;
    sf::Sprite heartSprite;
};
