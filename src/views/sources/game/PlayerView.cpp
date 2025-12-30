#include "../../headers/game/PlayerView.h"
#include <iostream>
#include <algorithm>

PlayerView::PlayerView()
{
    if (!textureStatic.loadFromFile("assets/animation/player/player_sheet.png"))
        std::cerr << "Erreur player_sheet.png\n";

    if (!textureWalkLeft.loadFromFile("assets/animation/player/player_walking_left.png"))
        std::cerr << "Erreur player_walking_left.png\n";

    if (!textureWalkRight.loadFromFile("assets/animation/player/player_walking_right.png"))
        std::cerr << "Erreur player_walking_right.png\n";

    if (!textureWalkFront.loadFromFile("assets/animation/player/player_walking_front.png"))
        std::cerr << "Erreur player_walking_front.png\n";

    if (!textureWalkBack.loadFromFile("assets/animation/player/player_walking_back.png"))
        std::cerr << "Erreur player_walking_back.png\n";

    textureWalkLeft.setSmooth(true);
    textureWalkRight.setSmooth(true);
    textureWalkFront.setSmooth(true);
    textureWalkBack.setSmooth(true);

    sprite.setTexture(textureStatic);

    // --- HUD HP ---
    hpBack.setSize({200.f, 18.f});
    hpBack.setFillColor(sf::Color(40, 40, 40, 220));

    hpFront.setSize({200.f, 18.f});
    hpFront.setFillColor(sf::Color::Green);

    hpOutline.setSize({200.f, 18.f});
    hpOutline.setFillColor(sf::Color::Transparent);
    hpOutline.setOutlineThickness(2.f);
    hpOutline.setOutlineColor(sf::Color::Black);
}

void PlayerView::playAnimation(
    sf::Texture& texture,
    int frameCount,
    int columns,
    float targetSize,
    bool animated
) {
    sprite.setTexture(texture);

    int frameSize = texture.getSize().x / columns;

    if (animated && animationTimer.getElapsedTime().asSeconds() > timePerFrame) {
        currentFrame = (currentFrame + 1) % frameCount;
        animationTimer.restart();
    }

    int col = currentFrame % columns;
    int row = currentFrame / columns;

    sprite.setTextureRect({
        col * frameSize,
        row * frameSize,
        frameSize,
        frameSize
    });

    float scale = targetSize / frameSize;
    sprite.setScale(scale, scale);
    sprite.setOrigin(frameSize / 2.f, frameSize / 2.f);
}

void PlayerView::render(sf::RenderWindow& window, const Player& player)
{
    sprite.setPosition(player.getPosition());

    Direction dir = player.getDirection();
    bool moving = player.isMoving();

    // --- ANIMATIONS ---
    if (dir == Direction::Left && moving)
        playAnimation(textureWalkLeft, 34, 6, 64.f, true);
    else if (dir == Direction::Right && moving)
        playAnimation(textureWalkRight, 33, 6, 64.f, true);
    else if (dir == Direction::Down && moving)
        playAnimation(textureWalkFront, 33, 6, 80.f, true);
    else if (dir == Direction::Up && moving)
        playAnimation(textureWalkBack, 33, 6, 80.f, true);
    else {
        // --- IDLE ---
        sprite.setTexture(textureStatic);

        sf::Vector2u size = textureStatic.getSize();
        float frameW = size.x / 4.f;
        float frameH = size.y;

        int frame = 0;
        switch (dir) {
            case Direction::Down:  frame = 0; break;
            case Direction::Left:  frame = 1; break;
            case Direction::Right: frame = 2; break;
            case Direction::Up:    frame = 3; break;
        }

        sprite.setTextureRect({
            static_cast<int>(frame * frameW),
            0,
            static_cast<int>(frameW),
            static_cast<int>(frameH)
        });

        float scale = 78.f / frameH;
        sprite.setScale(scale, scale);
        sprite.setOrigin(frameW / 2.f, frameH / 2.f);
        currentFrame = 0;
    }

    // --- FEEDBACK DÉGÂTS ---
    if (!player.isAlive())
        sprite.setColor(sf::Color(255, 255, 255, 80));
    else if (player.isInvincible())
        sprite.setColor(sf::Color(255, 100, 100));
    else
        sprite.setColor(sf::Color::White);

    window.draw(sprite);

    // ================= HUD =================
    float ratio = (float)player.getHealth() / player.getMaxHealth();
    ratio = std::clamp(ratio, 0.f, 1.f);

    if (ratio > 0.6f)
        hpFront.setFillColor(sf::Color::Green);
    else if (ratio > 0.3f)
        hpFront.setFillColor(sf::Color::Yellow);
    else
        hpFront.setFillColor(sf::Color::Red);

    hpFront.setSize({200.f * ratio, 18.f});

    sf::Vector2f hudPos(20.f, window.getSize().y - 40.f);
    hpBack.setPosition(hudPos);
    hpFront.setPosition(hudPos);
    hpOutline.setPosition(hudPos);

    window.draw(hpBack);
    window.draw(hpFront);
    window.draw(hpOutline);
}

