#pragma once
#include <SFML/Graphics.hpp>
#include "models/headers/Enemy.h"

class EnemyView {
public:
    EnemyView();

    void loadTextureForType(ZombieType type);
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
    sf::Texture bossZombieTexture;

    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    float healthBarWidth  = 30.f;
    float healthBarHeight = 5.f;

    int frameWidth = 0;
    int frameHeight = 0;

    ZombieType currentType;
    bool textureInitialized = false;
};