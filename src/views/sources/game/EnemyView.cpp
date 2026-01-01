#include "../../headers/game/EnemyView.h"
#include <iostream>
#include <cmath>

EnemyView::EnemyView() {
    if (!zombieBasicTexture.loadFromFile("assets/animation/Enemy/Basic/idle.png"))
        std::cerr << "Erreur Basic/idle.png\n";

    if (!zombieFastTexture.loadFromFile("assets/animation/Enemy/Fast/idle.png"))
        std::cerr << "Erreur Fast/idle.png\n";

    if (!zombieTankTexture.loadFromFile("assets/animation/Enemy/Tank/idle.png"))
        std::cerr << "Erreur Tank/idle.png\n";

    if (!boss01Texture.loadFromFile("assets/animation/Boss/TralaleroTralala/idle.png"))
        std::cerr << "Erreur TralaleroTralala/idle.png\n";

    if (!boss02Texture.loadFromFile("assets/animation/Boss/ChimpanziniBananini/idle.png"))
        std::cerr << "Erreur ChimpanziniBananini/idle.png\n";

    if (!boss03Texture.loadFromFile("assets/animation/Boss/UdinDinDinDun/idle.png"))
        std::cerr << "Erreur UdinDinDinDun/idle.png\n";

    if (!finalBossTexture.loadFromFile("assets/animation/Boss/OscarTheCrackhead/idle.png"))
        std::cerr << "Erreur OscarTheCrackhead/idle.png\n";


    // --- BACK ---
    hbBack.setSize({ healthBarWidth - 2 * healthBarRadius, healthBarHeight });
    hbBack.setFillColor(sf::Color(30, 30, 30, 220));

    hbBackLeft.setRadius(healthBarRadius);
    hbBackRight.setRadius(healthBarRadius);
    hbBackLeft.setFillColor(hbBack.getFillColor());
    hbBackRight.setFillColor(hbBack.getFillColor());

    // --- FRONT ---
    hbFront.setFillColor(sf::Color::Green);
    hbFrontLeft.setRadius(healthBarRadius);
    hbFrontRight.setRadius(healthBarRadius);

    // --- OUTLINE ---
    hbOutline.setSize({ healthBarWidth - 2 * healthBarRadius, healthBarHeight });
    hbOutline.setFillColor(sf::Color::Transparent);
    hbOutline.setOutlineThickness(1.f);
    hbOutline.setOutlineColor(sf::Color::Black);

    hbOutlineLeft.setRadius(healthBarRadius);
    hbOutlineRight.setRadius(healthBarRadius);
    hbOutlineLeft.setFillColor(sf::Color::Transparent);
    hbOutlineRight.setFillColor(sf::Color::Transparent);
    hbOutlineLeft.setOutlineThickness(1.f);
    hbOutlineRight.setOutlineThickness(1.f);
    hbOutlineLeft.setOutlineColor(sf::Color::Black);
    hbOutlineRight.setOutlineColor(sf::Color::Black);

}

void EnemyView::loadTextureForType(EnemyType type) {
    if (textureInitialized && type == currentType)
        return;

    currentType = type;
    textureInitialized = true;

    switch (type) {
        case EnemyType::Basic:
            sprite.setTexture(zombieBasicTexture);
            break;
        case EnemyType::Fast:
            sprite.setTexture(zombieFastTexture);
            break;
        case EnemyType::Tank:
            sprite.setTexture(zombieTankTexture);
            break;
        case EnemyType::Boss01:
            sprite.setTexture(boss01Texture);
            break;
        case EnemyType::Boss02:
            sprite.setTexture(boss02Texture);
            break;
        case EnemyType::Boss03:
            sprite.setTexture(boss03Texture);
            break;
        case EnemyType::FinalBoss:
            sprite.setTexture(finalBossTexture);
            break;
    }

    frameWidth  = sprite.getTexture()->getSize().x / 4;
    frameHeight = sprite.getTexture()->getSize().y;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
}

void EnemyView::drawHealthBar(sf::RenderWindow& window, const Enemy& enemy)
{
    float maxHp = enemy.getMaxHealth();
    if (maxHp <= 0.f) return;

    float ratio = enemy.getHealth() / maxHp;
    ratio = std::clamp(ratio, 0.f, 1.f);

    sf::Vector2f basePos = enemy.getPosition();
    float yOffset = enemy.getRadius() + 12.f;

    sf::Vector2f pos(
        basePos.x - healthBarWidth / 2.f,
        basePos.y - yOffset
    );

    // ---------------- BACK ----------------
    hbBack.setPosition(pos.x + healthBarRadius, pos.y);
    hbBackLeft.setPosition(pos.x, pos.y);
    hbBackRight.setPosition(
        pos.x + healthBarWidth - 2 * healthBarRadius,
        pos.y
    );

    // ---------------- FRONT ----------------
    float frontWidth = (healthBarWidth - 2 * healthBarRadius) * ratio;
    hbFront.setSize({ frontWidth, healthBarHeight });
    hbFront.setPosition(pos.x + healthBarRadius, pos.y);

    hbFrontLeft.setPosition(pos.x, pos.y);

    if (ratio > 0.f) {
        hbFrontRight.setPosition(
            pos.x + healthBarRadius + frontWidth - healthBarRadius,
            pos.y
        );
    }

    // Couleur dynamique
    if (ratio > 0.6f)
        hbFront.setFillColor(sf::Color(80, 220, 100));
    else if (ratio > 0.3f)
        hbFront.setFillColor(sf::Color(240, 200, 80));
    else
        hbFront.setFillColor(sf::Color(220, 80, 80));

    hbFrontLeft.setFillColor(hbFront.getFillColor());
    hbFrontRight.setFillColor(hbFront.getFillColor());

    // ---------------- OUTLINE ----------------
    hbOutline.setPosition(pos.x + healthBarRadius, pos.y);
    hbOutlineLeft.setPosition(pos.x, pos.y);
    hbOutlineRight.setPosition(
        pos.x + healthBarWidth - 2 * healthBarRadius,
        pos.y
    );

    // ---------------- DRAW ORDER ----------------
    window.draw(hbBackLeft);
    window.draw(hbBack);
    window.draw(hbBackRight);

    if (ratio > 0.f) {
        window.draw(hbFrontLeft);
        window.draw(hbFront);
        if (frontWidth > healthBarRadius)
            window.draw(hbFrontRight);
    }

    window.draw(hbOutlineLeft);
    window.draw(hbOutline);
    window.draw(hbOutlineRight);
}


void EnemyView::render(
    sf::RenderWindow& window,
    const Enemy& enemy,
    const sf::Vector2f& playerPos
) {
    // --- Sélection de la bonne texture (une seule fois) ---
    loadTextureForType(enemy.getType());

    // --- Direction selon la vélocité ---
    sf::Vector2f velocity = enemy.getVelocity();
    int dirIndex = 0;

    if (std::abs(velocity.x) > std::abs(velocity.y)) {
        dirIndex = (velocity.x < 0.f) ? 1 : 2;
    } else {
        dirIndex = (velocity.y < 0.f) ? 3 : 0;
    }

    // --- Frame directionnelle ---
    sprite.setTextureRect(sf::IntRect(
        dirIndex * frameWidth,
        0,
        frameWidth,
        frameHeight
    ));

    // --- Position ---
    sprite.setPosition(enemy.getPosition());

    // --- Scale basé sur le radius ---
    float scale = (enemy.getRadius() * 2.f) / frameWidth;
    sprite.setScale(scale, scale);

    // --- Dessin du zombie ---
    window.draw(sprite);

    // --- Affichage de la barre de vie si le joueur est proche ---
    sf::Vector2f diff = enemy.getPosition() - playerPos;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    const float SHOW_DISTANCE = 150.f;

    if (distance < SHOW_DISTANCE && enemy.isAlive()) {
        drawHealthBar(window, enemy);
    }
}
