#ifndef ZOMBIEHIGHSCHOOLBRAINROT_ENEMYVIEW_H
#define ZOMBIEHIGHSCHOOLBRAINROT_ENEMYVIEW_H

#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/Enemy.h"

class EnemyView {
public:
    EnemyView();

    void render(sf::RenderWindow& window, const Enemy& enemy);

private:
    sf::CircleShape shape;
};

#endif //ZOMBIEHIGHSCHOOLBRAINROT_ENEMYVIEW_H