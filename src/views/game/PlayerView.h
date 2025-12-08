#pragma once
#include <SFML/Graphics.hpp>
#include "../../models/Player.h"

class PlayerView {
public:
    PlayerView();

    // La méthode pour dessiner prend le Player pour savoir où il est et où il regarde
    void render(sf::RenderWindow& window, const Player& player);

private:
    sf::Texture texture; // L'image chargée en mémoire
    sf::Sprite sprite;   // L'objet qui affiche l'image
};