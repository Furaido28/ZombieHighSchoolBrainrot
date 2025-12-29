#include "../../headers/game/PlayerView.h"
#include <iostream>

PlayerView::PlayerView() {
    // -----------------------------------------------------------
    // 1. CHARGEMENT DE LA PLANCHE STATIQUE (Votre code de base)
    // -----------------------------------------------------------
    if (!textureStatic.loadFromFile("assets/player_sheet.png")) {
        std::cerr << "ERREUR: Impossible de charger assets/player_sheet.png" << std::endl;
    }

    // -----------------------------------------------------------
    // 2. CHARGEMENT DE L'ANIMATION MARCHE GAUCHE (Nouvelle image)
    // -----------------------------------------------------------
    // Assurez-vous d'avoir renommé l'image et qu'elle est bien en .png
    if (!textureWalkLeft.loadFromFile("assets/player_walking_left.png")) {
        std::cerr << "ERREUR: Impossible de charger assets/player_walking_left.png" << std::endl;
    }
    // ACTIVATION DU LISSAGE : Crucial car on réduit une image de 512px à 64px
    textureWalkLeft.setSmooth(true);

    // Initialisation par défaut
    sprite.setTexture(textureStatic);

    // Configuration de l'animation
    currentFrame = 0;
    timePerFrame = 0.03f; // Vitesse de l'animation (0.03s = ~30 fps)
}

void PlayerView::render(sf::RenderWindow& window, const Player& player) {
    // Mise à jour de la position
    sprite.setPosition(player.getPosition());

    Direction dir = player.getDirection();
    bool isMoving = player.isMoving();

    // =================================================================================
    // CAS 1 : LE JOUEUR MARCHE VERS LA GAUCHE (Animation Complexe)
    // =================================================================================
    if (dir == Direction::Left && isMoving) {

        // 1. On applique la texture haute définition
        sprite.setTexture(textureWalkLeft);

        // 2. Gestion du temps (Timer)
        if (animationTimer.getElapsedTime().asSeconds() > timePerFrame) {
            currentFrame++;
            // On a 34 images (de 0 à 33), donc si on dépasse 33, on revient à 0
            if (currentFrame >= 34) {
                currentFrame = 0;
            }
            animationTimer.restart();
        }

        // 3. Calcul de la grille (6 colonnes x 6 lignes)
        // L'image fait 3072x3072. 3072 / 6 = 512 pixels par case.
        int frameSize = 512;

        int col = currentFrame % 6; // Colonne actuelle
        int row = currentFrame / 6; // Ligne actuelle

        // Découpage du rectangle
        sf::IntRect rect(col * frameSize, row * frameSize, frameSize, frameSize);
        sprite.setTextureRect(rect);

        // 4. Mise à l'échelle (Scaling)
        // On veut que le perso fasse 64px de haut dans le jeu, mais l'image fait 512px.
        // Facteur = 64 / 512 = 0.125
        float scaleFactor = 64.f / (float)frameSize;
        sprite.setScale(scaleFactor, scaleFactor);

        // 5. Origine au centre (512 / 2 = 256)
        sprite.setOrigin(frameSize / 2.f, frameSize / 2.f);
    }

    // =================================================================================
    // CAS 2 : TOUS LES AUTRES CAS (Votre code de base adapté)
    // =================================================================================
    else {
        // 1. On remet la texture statique
        sprite.setTexture(textureStatic);

        // 2. IMPORTANT : On remet l'échelle normale !
        // (Sinon le sprite statique sera minuscule à cause du 0.125 précédent)

        // Calcul pour votre sprite statique (supposons qu'il doive faire 64px de haut aussi)
        sf::Vector2u staticSize = textureStatic.getSize();
        float staticFrameW = staticSize.x / 4.f;
        float staticFrameH = staticSize.y;

        // Si votre sprite sheet de base est petit (ex: 48px), on l'agrandit peut-être un peu
        // ou on le laisse à échelle 1. Ajustez "targetSize" selon vos goûts.
        float targetSize = 78.f;
        float scaleFactor = targetSize / staticFrameH;
        sprite.setScale(scaleFactor, scaleFactor);

        // 3. On remet l'origine au centre du sprite statique
        sprite.setOrigin(staticFrameW / 2.f, staticFrameH / 2.f);

        // 4. Sélection de la frame statique (Votre switch)
        int frameIndex = 0;
        switch(dir) {
            case Direction::Down:  frameIndex = 0; break;
            case Direction::Left:  frameIndex = 1; break;
            case Direction::Right: frameIndex = 2; break;
            case Direction::Up:    frameIndex = 3; break;
        }

        sf::IntRect rect(frameIndex * (int)staticFrameW, 0, (int)staticFrameW, (int)staticFrameH);
        sprite.setTextureRect(rect);
    }

    window.draw(sprite);
}