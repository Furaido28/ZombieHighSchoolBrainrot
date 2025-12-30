#pragma once
#include <SFML/Graphics.hpp>
#include "models/headers/Enemy.h"

class EnemyView {
public:
    EnemyView();
    void render(sf::RenderWindow& window, const Enemy& enemy);

private:
    void loadTextureForType(ZombieType type);

private:
    sf::Sprite sprite;

    sf::Texture zombieBasicTexture;
    sf::Texture zombieFastTexture;
    sf::Texture zombieTankTexture;
    sf::Texture bossZombieTexture;

    int frameWidth = 0;
    int frameHeight = 0;

    ZombieType currentType;
    bool textureInitialized = false;
};
