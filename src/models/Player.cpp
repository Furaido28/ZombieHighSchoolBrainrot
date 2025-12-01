#include "Player.h"

Player::Player()
    : position(0.f, 0.f), speed(200.f) {}

void Player::move(const sf::Vector2f& delta) {
    position += delta;
}

void Player::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
}

void Player::update(float dt) {
    // Logique future (animations, etc.)
}