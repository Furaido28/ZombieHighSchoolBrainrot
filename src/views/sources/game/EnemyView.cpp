#include "../../headers/game/EnemyView.h"
#include <iostream>
#include <cmath>

EnemyView::EnemyView() {
    if (!zombieBasicTexture.loadFromFile("assets/animation/ZombieBasic/zombie_basic.png")) {
        std::cerr << "Erreur chargement zombie_basic.png\n";
    }

    sprite.setTexture(zombieBasicTexture);

    frameWidth  = zombieBasicTexture.getSize().x / 4;
    frameHeight = zombieBasicTexture.getSize().y;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);

    // 🔍 DEBUG CRUCIAL
    std::cout << "Zombie texture: "
              << zombieBasicTexture.getSize().x << "x"
              << zombieBasicTexture.getSize().y << std::endl;
}

void EnemyView::render(sf::RenderWindow& window, const Enemy& enemy)
{
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
