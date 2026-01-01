#include "../../headers/game/PlayerView.h"
#include <iostream>
#include <algorithm>

#include "models/headers/Player.h"

PlayerView::PlayerView() {
    // --- Textures ---
    if (!textureIdle.loadFromFile("assets/animation/player/player_sheet.png"))
        std::cerr << "Erreur player_sheet.png\n";

    if (!textureLeft.loadFromFile("assets/animation/player/player_walking_left.png"))
        std::cerr << "Erreur walking_left.png\n";

    if (!textureRight.loadFromFile("assets/animation/player/player_walking_right.png"))
        std::cerr << "Erreur walking_right.png\n";

    if (!textureDown.loadFromFile("assets/animation/player/player_walking_front.png"))
        std::cerr << "Erreur walking_front.png\n";

    if (!textureUp.loadFromFile("assets/animation/player/player_walking_back.png"))
        std::cerr << "Erreur walking_back.png\n";

    textureLeft.setSmooth(true);
    textureRight.setSmooth(true);
    textureUp.setSmooth(true);
    textureDown.setSmooth(true);

    sprite.setTexture(textureIdle);

    // --- HUD ---
    hpBack.setSize({200.f, 20.f});
    hpBack.setFillColor(sf::Color(30, 30, 30, 220));

    hpFront.setSize({200.f, 20.f});
    hpFront.setFillColor(sf::Color::Green);

    hpOutline.setSize({200.f, 20.f});
    hpOutline.setFillColor(sf::Color::Transparent);
    hpOutline.setOutlineThickness(2.f);
    hpOutline.setOutlineColor(sf::Color::Black);

    if (!hudFont.loadFromFile("assets/fonts/arial.ttf"))
        std::cerr << "❌ Erreur police HUD (healthbar)\n";

    hpText.setFont(hudFont);
    hpText.setCharacterSize(14);
    hpText.setFillColor(sf::Color::White);
    hpText.setStyle(sf::Text::Bold);

    if (!heartTexture.loadFromFile("assets/ui/heart.png"))
        std::cerr << "Erreur heart.png\n";

    heartTexture.setSmooth(true);
    heartSprite.setTexture(heartTexture);

    // Taille du cœur
    float targetHeartSize = 18.f;

    sf::Vector2u texSize = heartTexture.getSize();
    float scale = targetHeartSize / texSize.x;

    heartSprite.setScale(scale, scale);

    waveText.setFont(hudFont);
    waveText.setCharacterSize(18);
    waveText.setFillColor(sf::Color::White);
    waveText.setStyle(sf::Text::Bold);

    timerText.setFont(hudFont);
    timerText.setCharacterSize(16);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
}

void PlayerView::playAnimation(
    sf::Texture& texture,
    int frameCount,
    int columns,
    float targetSize
) {
    sprite.setTexture(texture);

    int frameSize = texture.getSize().x / columns;

    if (animationClock.getElapsedTime().asSeconds() > frameTime) {
        currentFrame = (currentFrame + 1) % frameCount;
        animationClock.restart();
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


void PlayerView::renderWorld(sf::RenderWindow& window, const Player& player) {
    sprite.setPosition(player.getPosition());

    Direction dir = player.getDirection();
    bool moving = player.isMoving();

    if (moving) {
        if (dir == Direction::Left)
            playAnimation(textureLeft, 34, 6, 64.f);
        else if (dir == Direction::Right)
            playAnimation(textureRight, 33, 6, 64.f);
        else if (dir == Direction::Down)
            playAnimation(textureDown, 33, 6, 80.f);
        else if (dir == Direction::Up)
            playAnimation(textureUp, 33, 6, 80.f);
    }
    else {
        sprite.setTexture(textureIdle);

        sf::Vector2u size = textureIdle.getSize();
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

        sprite.setScale(78.f / frameH, 78.f / frameH);
        sprite.setOrigin(frameW / 2.f, frameH / 2.f);
        currentFrame = 0;
    }

    // Feedback dégâts
    if (!player.isAlive())
        sprite.setColor(sf::Color(255, 255, 255, 80));
    else if (player.isInvincible())
        sprite.setColor(sf::Color(255, 100, 100));
    else
        sprite.setColor(sf::Color::White);

    window.draw(sprite);
}

void PlayerView::renderHUD(sf::RenderWindow& window, const Player& player, int waveNumber, float timeLeft) {
    float ratio = std::clamp(
        (float)player.getHealth() / player.getMaxHealth(),
        0.f, 1.f
    );

    float margin = 20.f;
    float barWidth = 200.f;
    float barHeight = 20.f;

    sf::Vector2u winSize = window.getSize();
    sf::Vector2f pos(
        winSize.x - barWidth - margin,
        winSize.y - barHeight - margin
    );

    hpBack.setPosition(pos);
    hpFront.setPosition(pos);
    hpOutline.setPosition(pos);

    hpFront.setSize({barWidth * ratio, barHeight});

    if (ratio > 0.6f)
        hpFront.setFillColor(sf::Color::Green);
    else if (ratio > 0.3f)
        hpFront.setFillColor(sf::Color::Yellow);
    else
        hpFront.setFillColor(sf::Color::Red);

    // Texte HP
    hpText.setString(
        std::to_string((int)player.getHealth()) + " / " +
        std::to_string((int)player.getMaxHealth())
    );

    // Origine du texte : bas-centre
    sf::FloatRect textBounds = hpText.getLocalBounds();
    hpText.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height
    );

    // Espace entre texte et barre
    float textSpacing = 8.f;

    // Position du texte (au-dessus de la barre)
    hpText.setPosition(
        pos.x + barWidth / 2.f,
        pos.y - textSpacing
    );

    // --- CŒUR ---
    float heartSpacing = 6.f;
    sf::FloatRect heartBounds = heartSprite.getGlobalBounds();

    // Position du cœur à gauche du texte
    heartSprite.setPosition(
        hpText.getGlobalBounds().left - heartBounds.width - heartSpacing,
        hpText.getGlobalBounds().top +
        hpText.getGlobalBounds().height / 2.f -
        heartBounds.height / 2.f
    );

    // =========================
    // TEXTE DE VAGUE
    // =========================
    std::string waveLabel;

    switch (waveNumber) {
        case 1:
            waveLabel = "Simple WAVE";
            break;
        case 2:
            waveLabel = "Medium WAVE";
            break;
        case 3:
            waveLabel = "Difficult WAVE";
            break;
        case 4:
            waveLabel = "Hard WAVE";
            break;
        default:
            waveLabel = "Boss Wave";
    }

    waveText.setString(waveLabel);

    sf::Vector2u waveTextwinSize = window.getSize();
    float waveTextmargin = 20.f;

    waveText.setPosition(
        waveTextwinSize.x - waveText.getLocalBounds().width - waveTextmargin,
        waveTextmargin
    );

    window.draw(waveText);

    window.draw(hpBack);
    window.draw(hpFront);
    window.draw(hpOutline);
    window.draw(heartSprite);
    window.draw(hpText);

    // =========================
    // TIMER DE VAGUE
    // =========================
    int seconds = static_cast<int>(timeLeft);
    int minutes = seconds / 60;
    seconds %= 60;

    std::string timerStr =
        (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds);

    timerText.setString(timerStr);

    sf::FloatRect bounds = timerText.getLocalBounds();
    timerText.setOrigin(bounds.left + bounds.width, bounds.top);

    timerText.setPosition(
        window.getSize().x - 20.f,
        waveText.getPosition().y + waveText.getCharacterSize() + 5.f
    );

    window.draw(timerText);
}