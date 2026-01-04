#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Enemy.h"
#include <map>
#include <string>

// --- ANIMATION STRUCTURES ---
// Data for one direction animation (texture + frames)
struct DirectionData {
    sf::Texture texture;
    int frameCount = 1;
    int cols = 1;
};

// All animations for one enemy type
struct EnemyAnimations {
    DirectionData front;
    DirectionData back;
    DirectionData left;
    DirectionData right;

    float frameDuration = 0.15f; // Time per frame
    bool isLoaded = false;       // Avoid loading twice
};

class EnemyView {
public:
    EnemyView();

    // Draw enemy depending on its state and player position
    void render(
        sf::RenderWindow& window,
        const Enemy& enemy,
        const sf::Vector2f& playerPos
    );

private:
    // ================= SPRITE =================
    sf::Sprite sprite;

    // Animations for animated enemies (Basic, Fast, Tank)
    std::map<EnemyType, EnemyAnimations> animationsMap;
    sf::Clock globalClock;

    // Textures for boss enemies only (no animation)
    sf::Texture boss01Texture;
    sf::Texture boss02Texture;
    sf::Texture boss03Texture;
    sf::Texture finalBossTexture;

    int frameWidth  = 0;
    int frameHeight = 0;

    EnemyType currentType;
    bool textureInitialized = false;

    // Load textures based on enemy type
    void loadTextureForType(EnemyType type);

    // Load spritesheets for all directions
    void loadDirectionalTextures(
        EnemyType type,
        const std::string& folder,
        const std::string& filePrefix,
        int fFront,
        int fBack,
        int fLeft,
        int fRight,
        float speed,
        int cols = 6
    );

    // ================= HEALTH BAR =================
    // Draw enemy health bar above the sprite
    void drawHealthBar(sf::RenderWindow& window, const Enemy& enemy);

    float hpWidth  = 40.f;
    float hpHeight = 6.f;
    float hpRadius = hpHeight / 2.f;

    // Background shapes
    sf::RectangleShape hbBack;
    sf::RectangleShape hbFront;

    // Rounded corners (background)
    sf::CircleShape hbBackLeft;
    sf::CircleShape hbBackRight;

    // Rounded corners (foreground)
    sf::CircleShape hbFrontLeft;
    sf::CircleShape hbFrontRight;
};
