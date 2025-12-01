#include "MapView.h"

bool MapView::load(const std::string& tilesetPath, const TileMap& map) {
    if (!m_tileset.loadFromFile(tilesetPath))
        return false;

    // On utilise des Quads (carrés) pour dessiner les tuiles
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(map.getWidth() * map.getHeight() * 4);

    for (int i = 0; i < map.getWidth(); ++i) {
        for (int j = 0; j < map.getHeight(); ++j) {
            
            // On récupère l'info du MODÈLE
            int tileNumber = map.getTile(i, j);

            // Calcul des coordonnées dans la texture (Tileset)
            int tu = tileNumber % (m_tileset.getSize().x / map.getTileSize());
            int tv = tileNumber / (m_tileset.getSize().x / map.getTileSize());

            sf::Vertex* quad = &m_vertices[(i + j * map.getWidth()) * 4];

            // Position à l'écran
            quad[0].position = sf::Vector2f(i * map.getTileSize(), j * map.getTileSize());
            quad[1].position = sf::Vector2f((i + 1) * map.getTileSize(), j * map.getTileSize());
            quad[2].position = sf::Vector2f((i + 1) * map.getTileSize(), (j + 1) * map.getTileSize());
            quad[3].position = sf::Vector2f(i * map.getTileSize(), (j + 1) * map.getTileSize());

            // Coordonnées de texture
            quad[0].texCoords = sf::Vector2f(tu * map.getTileSize(), tv * map.getTileSize());
            quad[1].texCoords = sf::Vector2f((tu + 1) * map.getTileSize(), tv * map.getTileSize());
            quad[2].texCoords = sf::Vector2f((tu + 1) * map.getTileSize(), (tv + 1) * map.getTileSize());
            quad[3].texCoords = sf::Vector2f(tu * map.getTileSize(), (tv + 1) * map.getTileSize());
        }
    }
    return true;
}

void MapView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_tileset;
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}