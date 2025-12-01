#include "GameController.h"
#include <iostream>

GameController::GameController() {
    // --- 1. CHARGEMENT DE LA MAP ---
    if (!tileMap.loadFromFile("assets/maps/map1.txt")) {
        std::cerr << "ERREUR : Impossible de charger assets/maps/map1.txt" << std::endl;
    }

    if (!mapView.load("assets/tileset.png", tileMap)) {
        std::cerr << "ERREUR : Impossible de charger assets/tileset.png" << std::endl;
    }

    // --- 2. SPAWN DU JOUEUR ---
    // On place le joueur à la case (2, 2) soit 64px, 64px pour éviter les murs
    player.setPosition(64.f + 16.f, 64.f + 16.f);

    // --- 3. CONFIGURATION CAMÉRA ---
    // Zoom x2 (640x360) pour bien voir le pixel art
    gameView.setSize(960.f, 540.f);
    // gameView.setSize(640.f, 360.f);
    gameView.setCenter(player.getPosition());
}

void GameController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {

        sf::Vector2f moveAmount(0.f, 0.f);
        float step = 32.f; // On se déplace d'une case (32 pixels) à la fois

        // Détection de la touche
        if (event.key.code == sf::Keyboard::Z) moveAmount.y = -step;
        else if (event.key.code == sf::Keyboard::S) moveAmount.y = step;
        else if (event.key.code == sf::Keyboard::Q) moveAmount.x = -step;
        else if (event.key.code == sf::Keyboard::D) moveAmount.x = step;

        // Si on essaie de bouger
        if (moveAmount.x != 0 || moveAmount.y != 0) {

            // 1. Calculer la position FUTURE (le centre)
            sf::Vector2f nextPos = player.getPosition() + moveAmount;

            // 2. Créer un "Point de vérification" (CheckPoint)
            // Au lieu de vérifier le centre, on va vérifier le bord du cercle
            sf::Vector2f checkPos = nextPos;

            // Le rayon du joueur est de 14px. On vérifie 14px devant.
            // On peut réduire un peu (ex: 12px) si on veut être moins sévère.
            float offset = 14.f;

            if (moveAmount.x > 0) checkPos.x += offset; // On va à Droite -> Check bord Droit
            if (moveAmount.x < 0) checkPos.x -= offset; // On va à Gauche -> Check bord Gauche
            if (moveAmount.y > 0) checkPos.y += offset; // On va en Bas    -> Check bord Bas
            if (moveAmount.y < 0) checkPos.y -= offset; // On va en Haut   -> Check bord Haut

            // 3. Convertir ce point précis en coordonnées de grille
            int gridX = static_cast<int>(checkPos.x) / tileMap.getTileSize();
            int gridY = static_cast<int>(checkPos.y) / tileMap.getTileSize();

            // 4. Vérifier la collision
            if (tileMap.getTile(gridX, gridY) == 0) {
                player.move(moveAmount);
            } else {
                std::cout << "Bloqué ! Le bord du joueur touche un mur." << std::endl;
            }
        }
    }
}

void GameController::update(float dt) {
    player.update(dt);
}

void GameController::render(sf::RenderWindow& window) {
    // --- 1. MISE À JOUR CAMÉRA ---
    // La caméra se centre sur le joueur
    gameView.setCenter(player.getPosition());
    window.setView(gameView);

    // --- 2. DESSIN ---
    window.draw(mapView);            // Carte en dessous
    playerView.render(window, player); // Joueur au dessus
}