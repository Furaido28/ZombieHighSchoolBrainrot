#include "PlayerView.h"

PlayerView::PlayerView() {
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(20.f, 20.f);
}

void PlayerView::render(sf::RenderWindow& window, const Player& player) {
    shape.setPosition(player.getPosition());
    window.draw(shape);
}
