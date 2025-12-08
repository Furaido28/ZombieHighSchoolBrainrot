#include "MapView.h"
#include <iostream>

bool MapView::load(const TileMap& map) {
    // 1. CHARGEMENT DE L'IMAGE
    // Assurez-vous que l'image est bien dans assets/tileset.png
    if (!m_tileset.loadFromFile("assets/tileset.png")) {
        std::cerr << "ERREUR: Impossible de charger assets/tileset.png" << std::endl;
        return false;
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(map.getWidth() * map.getHeight() * 4);

    float ts = (float)map.getTileSize(); // 64.0f

    // Calcul pour savoir combien de tuiles il y a par ligne dans votre image
    int tilesPerRow = m_tileset.getSize().x / (int)ts;

    for (unsigned i = 0; i < map.getWidth(); ++i) {
        for (unsigned j = 0; j < map.getHeight(); ++j) {

            // 2. CHOIX DE LA TUILE (MAPPING)
            char tileType = map.getTile(i, j);

            // C'est ici que vous décidez quelle image correspond à quel caractère
            // Index 0 = La première tuile en haut à gauche de l'image
            // Index 1 = La tuile à droite de la première, etc.
            int tileIndex = 0;

            if (tileType == '.')      tileIndex = 0; // Sol = 1ère image
            else if (tileType == '#') tileIndex = 1; // Mur = 2ème image
            else                      tileIndex = 2; // Autre...

            // 3. CALCUL DES COORDONNÉES DE TEXTURE (Maths)
            // On trouve la colonne (tu) et la ligne (tv) dans l'image
            int tu = tileIndex % tilesPerRow;
            int tv = tileIndex / tilesPerRow;

            sf::Vertex* quad = &m_vertices[(i + j * map.getWidth()) * 4];

            // --- POSITION À L'ÉCRAN (Ça ne change pas) ---
            quad[0].position = sf::Vector2f(i * ts, j * ts);
            quad[1].position = sf::Vector2f((i + 1) * ts, j * ts);
            quad[2].position = sf::Vector2f((i + 1) * ts, (j + 1) * ts);
            quad[3].position = sf::Vector2f(i * ts, (j + 1) * ts);

            // --- COORDONNÉES DE TEXTURE (Nouveau !) ---
            float u = (float)(tu * (int)ts);
            float v = (float)(tv * (int)ts);

            quad[0].texCoords = sf::Vector2f(u, v);
            quad[1].texCoords = sf::Vector2f(u + ts, v);
            quad[2].texCoords = sf::Vector2f(u + ts, v + ts);
            quad[3].texCoords = sf::Vector2f(u, v + ts);

            // On remet la couleur à BLANC pour ne pas teinter l'image
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

    // IMPORTANT : On active la texture !
    states.texture = &m_tileset;

    target.draw(m_vertices, states);
}