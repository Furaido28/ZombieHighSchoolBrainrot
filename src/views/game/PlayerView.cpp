#include "PlayerView.h"
#include <iostream>

PlayerView::PlayerView() {
    // 1. Chargement de l'image (Spritesheet)
    // Assurez-vous que le fichier s'appelle bien "player_sheet.png" dans "assets"
    if (!texture.loadFromFile("assets/player_sheet.png")) {
        std::cerr << "ERREUR: Impossible de charger assets/player_sheet.png" << std::endl;
        // En cas d'erreur, on pourrait charger une couleur par défaut,
        // mais ici le sprite restera blanc/vide.
    }

    // 2. On applique la texture au sprite
    sprite.setTexture(texture);

    // 3. Calcul de la taille d'une seule frame
    // Votre image contient 4 persos alignés horizontalement.
    sf::Vector2u textureSize = texture.getSize();
    float frameWidth = (float)textureSize.x / 4.f; // On divise la largeur par 4
    float frameHeight = (float)textureSize.y;      // La hauteur est complète

    // 4. On place l'origine au CENTRE du personnage
    // C'est très important car votre Player (Modèle) a sa position au centre.
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
}

void PlayerView::render(sf::RenderWindow& window, const Player& player) {
    // --- POSITION ---
    // On récupère la position du modèle
    sprite.setPosition(player.getPosition());

    // --- ANIMATION (DIRECTION) ---
    // On récupère la direction du modèle (ajoutée dans l'étape précédente)
    Direction dir = player.getDirection();

    // On détermine quel index d'image utiliser (0, 1, 2 ou 3)
    // Rappel de votre ordre : 1:Face, 2:Gauche, 3:Droite, 4:Dos
    int frameIndex = 0;

    switch(dir) {
        case Direction::Down:  frameIndex = 0; break; // Face
        case Direction::Left:  frameIndex = 1; break; // Gauche
        case Direction::Right: frameIndex = 2; break; // Droite
        case Direction::Up:    frameIndex = 3; break; // Dos
    }

    // --- DÉCOUPAGE ---
    // On calcule le rectangle à afficher
    sf::Vector2u textureSize = texture.getSize();
    int frameW = textureSize.x / 4;
    int frameH = textureSize.y;

    // (X, Y, Largeur, Hauteur)
    // X change selon la direction (frameIndex * largeur)
    sf::IntRect rect(frameIndex * frameW, 0, frameW, frameH);

    sprite.setTextureRect(rect);

    // --- DESSIN ---
    window.draw(sprite);
}