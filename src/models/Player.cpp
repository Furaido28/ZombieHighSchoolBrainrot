#include "Player.h"

Player::Player()
    : position(400.f, 300.f), speed(200.f) {}

void Player::move(const sf::Vector2f& dir) {
    position += dir * speed;
}

void Player::update(float dt) {
    // future logic
}
