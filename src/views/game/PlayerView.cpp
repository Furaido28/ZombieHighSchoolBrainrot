#include "PlayerView.h"

PlayerView::PlayerView() {
    // 1. Taille : Rayon de 14px = Diamètre de 28px.
    m_shape.setRadius(14.f);

    // 2. Couleur : Cyan (vrai personnage plus tard et pas un cercle)
    m_shape.setFillColor(sf::Color::Cyan);

    // 3. Point d'origine : AU CENTRE
    m_shape.setOrigin(14.f, 14.f);
}

void PlayerView::render(sf::RenderWindow& window, const Player& player) {
    // On déplace le sprite visuel à la position du modèle (logique)
    m_shape.setPosition(player.getPosition());

    // On dessine
    window.draw(m_shape);
}