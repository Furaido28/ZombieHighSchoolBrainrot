#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Enemy.h"

class EnemyView {
public:
    EnemyView();

    void render(
        sf::RenderWindow& window,
        const Enemy& enemy,
        const sf::Vector2f& playerPos
    );

private:
    // ================= SPRITE =================
    sf::Sprite sprite;

    sf::Texture zombieBasicTexture;
    sf::Texture zombieFastTexture;
    sf::Texture zombieTankTexture;
    sf::Texture boss01Texture;
    sf::Texture boss02Texture;
    sf::Texture boss03Texture;
    sf::Texture finalBossTexture;

    int frameWidth  = 0;
    int frameHeight = 0;

    EnemyType currentType;
    bool textureInitialized = false;

    void loadTextureForType(EnemyType type);

    // ================= HEALTH BAR =================
    void drawHealthBar(sf::RenderWindow& window, const Enemy& enemy);

    float hpWidth  = 40.f;
    float hpHeight = 6.f;
    float hpRadius = hpHeight / 2.f;

    sf::RectangleShape hbBack;
    sf::RectangleShape hbFront;

    sf::CircleShape hbBackLeft;
    sf::CircleShape hbBackRight;

    sf::CircleShape hbFrontLeft;
    sf::CircleShape hbFrontRight;
};