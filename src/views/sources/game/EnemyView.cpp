#include "../../headers/game/EnemyView.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// ======================================================
// CONSTRUCTOR
// ======================================================
EnemyView::EnemyView()
{
    // ---------- Textures ----------
    if (!zombieBasicTexture.loadFromFile("assets/animation/Enemy/Basic/idle.png"))
        std::cerr << "Erreur Basic idle\n";

    if (!zombieFastTexture.loadFromFile("assets/animation/Enemy/Fast/idle.png"))
        std::cerr << "Erreur Fast idle\n";

    if (!zombieTankTexture.loadFromFile("assets/animation/Enemy/Tank/idle.png"))
        std::cerr << "Erreur Tank idle\n";

    if (!boss01Texture.loadFromFile("assets/animation/Boss/TralaleroTralala/idle.png"))
        std::cerr << "Erreur Boss01 idle\n";

    if (!boss02Texture.loadFromFile("assets/animation/Boss/ChimpanziniBananini/idle.png"))
        std::cerr << "Erreur Boss02 idle\n";

    if (!boss03Texture.loadFromFile("assets/animation/Boss/UdinDinDinDun/idle.png"))
        std::cerr << "Erreur Boss03 idle\n";

    if (!finalBossTexture.loadFromFile("assets/animation/Boss/OscarTheCrackhead/idle.png"))
        std::cerr << "Erreur FinalBoss idle\n";

    // ---------- Health bar BACK ----------
    hbBack.setSize({ hpWidth - 2 * hpRadius, hpHeight });
    hbBack.setFillColor(sf::Color(30, 30, 30, 220));

    hbBackLeft.setRadius(hpRadius);
    hbBackRight.setRadius(hpRadius);
    hbBackLeft.setFillColor(hbBack.getFillColor());
    hbBackRight.setFillColor(hbBack.getFillColor());

    // ---------- Health bar FRONT ----------
    hbFront.setFillColor(sf::Color::Green);

    hbFrontLeft.setRadius(hpRadius);
    hbFrontRight.setRadius(hpRadius);
    hbFrontLeft.setFillColor(sf::Color::Green);
    hbFrontRight.setFillColor(sf::Color::Green);

    // ---------- Outline (noir) ----------
    hbBack.setOutlineThickness(1.f);
    hbBack.setOutlineColor(sf::Color::Black);

    hbBackLeft.setOutlineThickness(1.f);
    hbBackRight.setOutlineThickness(1.f);
    hbBackLeft.setOutlineColor(sf::Color::Black);
    hbBackRight.setOutlineColor(sf::Color::Black);
}

// ======================================================
// LOAD TEXTURE
// ======================================================
void EnemyView::loadTextureForType(EnemyType type)
{
    if (textureInitialized && type == currentType)
        return;

    currentType = type;
    textureInitialized = true;

    switch (type) {
        case EnemyType::Basic:     sprite.setTexture(zombieBasicTexture); break;
        case EnemyType::Fast:      sprite.setTexture(zombieFastTexture);  break;
        case EnemyType::Tank:      sprite.setTexture(zombieTankTexture);  break;
        case EnemyType::Boss01:    sprite.setTexture(boss01Texture);      break;
        case EnemyType::Boss02:    sprite.setTexture(boss02Texture);      break;
        case EnemyType::Boss03:    sprite.setTexture(boss03Texture);      break;
        case EnemyType::FinalBoss: sprite.setTexture(finalBossTexture);   break;
    }

    frameWidth  = sprite.getTexture()->getSize().x / 4;
    frameHeight = sprite.getTexture()->getSize().y;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
}

// ======================================================
// DRAW HEALTH BAR (ROUNDED RECT)
// ======================================================
void EnemyView::drawHealthBar(sf::RenderWindow& window, const Enemy& enemy)
{
    float maxHp = enemy.getMaxHealth();
    if (maxHp <= 0.f) return;

    float ratio = std::clamp(enemy.getHealth() / maxHp, 0.f, 1.f);

    sf::Vector2f basePos = enemy.getPosition();
    float yOffset = enemy.getRadius() + 10.f;

    sf::Vector2f pos(
        basePos.x - hpWidth / 2.f,
        basePos.y - yOffset
    );

    // ---------- BACK ----------
    hbBack.setPosition(pos.x + hpRadius, pos.y);
    hbBackLeft.setPosition(pos.x, pos.y);
    hbBackRight.setPosition(pos.x + hpWidth - 2 * hpRadius, pos.y);

    // ---------- FRONT ----------
    float fillWidth = (hpWidth - 2 * hpRadius) * ratio;

    hbFront.setSize({ fillWidth, hpHeight });
    hbFront.setPosition(pos.x + hpRadius, pos.y);

    hbFrontLeft.setPosition(pos.x, pos.y);

    if (fillWidth > hpRadius) {
        hbFrontRight.setPosition(
            pos.x + hpRadius + fillWidth - hpRadius,
            pos.y
        );
    }

    // ---------- Color ----------
    sf::Color color;
    if (ratio > 0.6f)
        color = sf::Color(80, 220, 100);
    else if (ratio > 0.3f)
        color = sf::Color(240, 200, 80);
    else
        color = sf::Color(220, 80, 80);

    hbFront.setFillColor(color);
    hbFrontLeft.setFillColor(color);
    hbFrontRight.setFillColor(color);

    // ---------- DRAW ----------
    window.draw(hbBackLeft);
    window.draw(hbBack);
    window.draw(hbBackRight);

    if (ratio > 0.f) {
        window.draw(hbFrontLeft);
        window.draw(hbFront);
        if (fillWidth > hpRadius)
            window.draw(hbFrontRight);
    }
}

// ======================================================
// RENDER
// ======================================================
void EnemyView::render(
    sf::RenderWindow& window,
    const Enemy& enemy,
    const sf::Vector2f& playerPos
) {
    loadTextureForType(enemy.getType());

    // ---------- Direction ----------
    sf::Vector2f v = enemy.getVelocity();
    int dirIndex = 0;

    if (std::abs(v.x) > std::abs(v.y))
        dirIndex = (v.x < 0.f) ? 1 : 2;
    else
        dirIndex = (v.y < 0.f) ? 3 : 0;

    sprite.setTextureRect({
        dirIndex * frameWidth,
        0,
        frameWidth,
        frameHeight
    });

    // ---------- Position / Scale ----------
    sprite.setPosition(enemy.getPosition());
    float scale = (enemy.getRadius() * 2.f) / frameWidth;
    sprite.setScale(scale, scale);

    window.draw(sprite);

    // ---------- Health bar distance check ----------
    sf::Vector2f d = enemy.getPosition() - playerPos;
    float dist = std::sqrt(d.x * d.x + d.y * d.y);

    if (dist < 150.f && enemy.isAlive())
        drawHealthBar(window, enemy);
}