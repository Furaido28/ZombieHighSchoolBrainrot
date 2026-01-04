#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "models/headers/Player.h"

class GameController;

class PlayerView {
public:
    PlayerView(GameController& controller);

    // Draw the player in the game world (with camera)
    void renderWorld(sf::RenderWindow& window, const Player& player);

    // Draw HUD elements (no camera)
    void renderHUD(
        sf::RenderWindow& window,
        const Player& player,
        int waveNumber,
        float timeLeft
    );

private:
    // Play sprite animation from a spritesheet
    void playAnimation(
        sf::Texture& texture,
        int frameCount,
        int columns,
        float targetSize
    );

    sf::Sprite sprite;

    // Player textures (directions & idle)
    sf::Texture textureIdle;
    sf::Texture textureLeft;
    sf::Texture textureRight;
    sf::Texture textureUp;
    sf::Texture textureDown;

    // Animation data
    sf::Clock animationClock;
    int currentFrame = 0;
    float frameTime = 0.03f;

    // ================= HUD =================
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;
    sf::RectangleShape hpOutline;

    sf::Text hpText;
    sf::Font hudFont;

    sf::Texture heartTexture;
    sf::Sprite heartSprite;

    sf::Text preWaveText;
    sf::Text waveText;
    sf::Text timerText;

    // Walking sound
    sf::SoundBuffer walkBuffer;
    sf::Sound walkSound;
    bool wasMoving = false;

    GameController& controller;
};