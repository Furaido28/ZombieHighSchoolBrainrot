#include "../../headers/game/EnemyView.h"

EnemyView::EnemyView()
{
    shape.setFillColor(sf::Color(100, 200, 100)); // vert zombie
}

void EnemyView::render(sf::RenderWindow& window, const Enemy& enemy)
{
    float r = enemy.getRadius();

    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setPosition(enemy.getPosition());

    window.draw(shape);
}
