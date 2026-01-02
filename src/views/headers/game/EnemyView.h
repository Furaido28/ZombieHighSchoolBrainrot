#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Enemy.h"
#include <map>
#include <string>

// --- STRUCTURES D'ANIMATION ---
struct DirectionData {
    sf::Texture texture;
    int frameCount = 1;
    int cols = 1;
};

struct EnemyAnimations {
    DirectionData front;
    DirectionData back;
    DirectionData left;
    DirectionData right;

    float frameDuration = 0.15f;
    bool isLoaded = false;
};

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

    // Map pour les ennemis animés (Basic, Fast, Tank)
    std::map<EnemyType, EnemyAnimations> animationsMap;
    sf::Clock globalClock;

    // Textures pour les Boss uniquement
    sf::Texture boss01Texture;
    sf::Texture boss02Texture;
    sf::Texture boss03Texture;
    sf::Texture finalBossTexture;

    int frameWidth  = 0;
    int frameHeight = 0;

    EnemyType currentType;
    bool textureInitialized = false;

    void loadTextureForType(EnemyType type);

    // Fonction de chargement des spritesheets
    void loadDirectionalTextures(EnemyType type, const std::string& folder, const std::string& filePrefix,
                                 int fFront, int fBack, int fLeft, int fRight, float speed, int cols = 6);

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