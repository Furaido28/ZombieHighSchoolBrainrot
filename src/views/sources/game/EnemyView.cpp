#include "../../headers/game/EnemyView.h"
#include <iostream>
#include <cmath>

EnemyView::EnemyView() {
    if (!zombieBasicTexture.loadFromFile("assets/animation/ZombieBasic/basic_idle.png"))
        std::cerr << "Erreur basic_idle.png\n";

    if (!zombieFastTexture.loadFromFile("assets/animation/ZombieFast/fast_idle.png"))
        std::cerr << "Erreur fast_idle.png\n";

    if (!zombieTankTexture.loadFromFile("assets/animation/ZombieTank/tank_idle.png"))
        std::cerr << "Erreur tank_idle.png\n";

    if (!bossZombieTexture.loadFromFile("assets/animation/BossZombie/boss_idle.png"))
        std::cerr << "Erreur boss_idle.png\n";

    healthBarBack.setSize({ healthBarWidth, healthBarHeight });
    healthBarBack.setFillColor(sf::Color(50, 50, 50, 200));

    healthBarFront.setSize({ healthBarWidth, healthBarHeight });
    healthBarFront.setFillColor(sf::Color::Green);
}

void EnemyView::loadTextureForType(ZombieType type) {
    if (textureInitialized && type == currentType)
        return;

    currentType = type;
    textureInitialized = true;

    switch (type) {
        case ZombieType::Basic:
            sprite.setTexture(zombieBasicTexture);
            break;
        case ZombieType::Fast:
            sprite.setTexture(zombieFastTexture);
            break;
        case ZombieType::Tank:
            sprite.setTexture(zombieTankTexture);
            break;
        case ZombieType::Boss:
            sprite.setTexture(bossZombieTexture);
            break;
    }

    frameWidth  = sprite.getTexture()->getSize().x / 4;
    frameHeight = sprite.getTexture()->getSize().y;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
}

void EnemyView::drawHealthBar(
    sf::RenderWindow& window,
    const Enemy& enemy
)
{
    float ratio = enemy.getHealth() / enemy.getMaxHealth();
    ratio = std::max(0.f, ratio);

    // Position au-dessus du zombie
    sf::Vector2f pos = enemy.getPosition();
    float yOffset = enemy.getRadius() + 10.f;

    healthBarBack.setPosition(
        pos.x - healthBarWidth / 2.f,
        pos.y - yOffset
    );

    healthBarFront.setPosition(healthBarBack.getPosition());
    healthBarFront.setSize({
        healthBarWidth * ratio,
        healthBarHeight
    });

    // Couleur dynamique
    if (ratio > 0.6f)
        healthBarFront.setFillColor(sf::Color::Green);
    else if (ratio > 0.3f)
        healthBarFront.setFillColor(sf::Color::Yellow);
    else
        healthBarFront.setFillColor(sf::Color::Red);

    window.draw(healthBarBack);
    window.draw(healthBarFront);
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
