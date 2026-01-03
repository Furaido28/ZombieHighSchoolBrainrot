#include "../../headers/game/MapView.h"
#include <iostream>
#include <cstdlib> // Nécessaire pour std::rand()

bool MapView::load(const TileMap& map) {
    if (!m_tileset.loadFromFile("assets/tileset.png")) {
        std::cerr << "ERREUR: Impossible de charger assets/tileset.png" << std::endl;
        return false;
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(map.getWidth() * map.getHeight() * 4);

    // --- CONFIGURATION TAILLES ---

    // 1. Taille Logique (Monde) : 64px
    float worldSize = (float)map.getTileSize();

    // 2. Taille Texture (Image) : 64px
    float textureSize = 64.0f;

    // Calcul du nombre de colonnes dans l'image
    int tilesPerRow = m_tileset.getSize().x / (int)textureSize;

    for (unsigned i = 0; i < map.getWidth(); ++i) {
        for (unsigned j = 0; j < map.getHeight(); ++j) {

            char tileType = map.getTile(i, j);
            int tileIndex = 0;

            // --- MAPPING & CHOIX DE TEXTURE ---
            if (tileType == '.')      tileIndex = 0; // SOL
            else if (tileType == '#') tileIndex = 1; // MUR
            else if (tileType == 'B') tileIndex = 2; // BANC
            else if (tileType == 'R') tileIndex = 3; // TABLE
            else if (tileType == 'N') tileIndex = 4; // NOIR
            else if (tileType == 'A') tileIndex = 5; // BANC ensanglanté
            else if (tileType == '*') tileIndex = 10; // Grass
            else if (tileType == 'T') tileIndex = 11; // Arbre
            else if (tileType == 'r') tileIndex = 11; // Roche

            // --- GESTION DU SANG ALEATOIRE ---
            else if (tileType == ',') {
                // 1. Liste ici les index de tes images de sang dans le tileset
                //    (Même si elles ne sont pas côte à côte)
                //    Exemple : si tes images sont aux positions 6, 8 et 12.
                int bloodIndices[] = { 6, 7, 8, 9 };

                // 2. On compte combien il y a de choix (ici 3)
                int numberOfVariants = 4;

                // 3. On en choisit un au hasard
                int randomIndex = std::rand() % numberOfVariants;
                tileIndex = bloodIndices[randomIndex];
            }
            // ----------------------------------

            else if (
                tileType == '$' ||
                tileType == '1' ||
                tileType == '2' ||
                tileType == '3'
                ) tileIndex = 0; // Les items spawn sur du sol standard

            // Calcul colonne/ligne dans le tileset
            int tu = tileIndex % tilesPerRow;
            int tv = tileIndex / tilesPerRow;

            sf::Vertex* quad = &m_vertices[(i + j * map.getWidth()) * 4];

            // --- A. POSITION ---
            quad[0].position = sf::Vector2f(i * worldSize, j * worldSize);
            quad[1].position = sf::Vector2f((i + 1) * worldSize, j * worldSize);
            quad[2].position = sf::Vector2f((i + 1) * worldSize, (j + 1) * worldSize);
            quad[3].position = sf::Vector2f(i * worldSize, (j + 1) * worldSize);

            // --- B. TEXTURE & ROTATION ---
            // Coordonnées de base dans l'image (selon l'index choisi plus haut)
            float u = (float)(tu * (int)textureSize);
            float v = (float)(tv * (int)textureSize);

            // Définition des 4 coins de la texture
            sf::Vector2f topLeft     = sf::Vector2f(u, v);
            sf::Vector2f topRight    = sf::Vector2f(u + textureSize, v);
            sf::Vector2f bottomRight = sf::Vector2f(u + textureSize, v + textureSize);
            sf::Vector2f bottomLeft  = sf::Vector2f(u, v + textureSize);

            // Par défaut (Rotation 0°)
            sf::Vector2f t0 = topLeft;      // Haut-Gauche
            sf::Vector2f t1 = topRight;     // Haut-Droite
            sf::Vector2f t2 = bottomRight;  // Bas-Droite
            sf::Vector2f t3 = bottomLeft;   // Bas-Gauche

            // ROTATION ALÉATOIRE (Seulement pour le sang ',')
            // Cela s'ajoute au choix de l'image pour encore plus de variété !
            if (tileType == ',') {
                int rotation = std::rand() % 4; // 0, 1, 2 ou 3

                if (rotation == 1) { // 90°
                    t0 = bottomLeft; t1 = topLeft; t2 = topRight; t3 = bottomRight;
                }
                else if (rotation == 2) { // 180°
                    t0 = bottomRight; t1 = bottomLeft; t2 = topLeft; t3 = topRight;
                }
                else if (rotation == 3) { // 270°
                    t0 = topRight; t1 = bottomRight; t2 = bottomLeft; t3 = topLeft;
                }
            }

            // Application des coordonnées de texture
            quad[0].texCoords = t0;
            quad[1].texCoords = t1;
            quad[2].texCoords = t2;
            quad[3].texCoords = t3;

            // Couleur
            quad[0].color = sf::Color::White;
            quad[1].color = sf::Color::White;
            quad[2].color = sf::Color::White;
            quad[3].color = sf::Color::White;
        }
    }
    return true;
}

void MapView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}