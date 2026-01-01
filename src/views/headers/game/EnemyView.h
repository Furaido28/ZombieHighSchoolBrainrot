#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Enemy.h"

class EnemyView {
public:
    EnemyView();

    void loadTextureForType(EnemyType type);
    void drawHealthBar(sf::RenderWindow& window, const Enemy& enemy);
    void render(
        sf::RenderWindow& window,
        const Enemy& enemy,
        const sf::Vector2f& playerPos);

private:
    sf::Sprite sprite;

    sf::Texture zombieBasicTexture;
    sf::Texture zombieFastTexture;
    sf::Texture zombieTankTexture;
    sf::Texture boss01Texture;
    sf::Texture boss02Texture;
    sf::Texture boss03Texture;
    sf::Texture finalBossTexture;


    // Barre de vie stylée
    float healthBarWidth  = 36.f;
    float healthBarHeight = 6.f;
    float healthBarRadius = 3.f;

    sf::RectangleShape hbBack;
    sf::RectangleShape hbFront;
    sf::RectangleShape hbOutline;

    sf::CircleShape hbBackLeft, hbBackRight;
    sf::CircleShape hbFrontLeft, hbFrontRight;
    sf::CircleShape hbOutlineLeft, hbOutlineRight;

    int frameWidth = 0;
    int frameHeight = 0;

    EnemyType currentType;
    bool textureInitialized = false;
};