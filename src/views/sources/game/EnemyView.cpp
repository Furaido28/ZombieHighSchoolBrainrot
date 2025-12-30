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
}

void EnemyView::loadTextureForType(ZombieType type)
{
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

void EnemyView::render(sf::RenderWindow& window, const Enemy& enemy)
{
    loadTextureForType(enemy.getType());

    sf::Vector2f velocity = enemy.getVelocity();
    int dirIndex = 0;

    if (std::abs(velocity.x) > std::abs(velocity.y)) {
        dirIndex = (velocity.x < 0) ? 1 : 2;
    } else {
        dirIndex = (velocity.y < 0) ? 3 : 0;
    }

    sprite.setTextureRect(sf::IntRect(
        dirIndex * frameWidth,
        0,
        frameWidth,
        frameHeight
    ));

    sprite.setPosition(enemy.getPosition());

    float scale = (enemy.getRadius() * 2.f) / frameWidth;
    sprite.setScale(scale, scale);

    window.draw(sprite);
}
