#pragma once

#include <SFML/Graphics.hpp>
#include "../../../models/headers/TileMap.h"

class MapView : public sf::Drawable, public sf::Transformable {
public:
    // Load the tile map data and build the vertex array
    bool load(const TileMap& map);

private:
    // Draw the map using SFML drawable interface
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices; // All tiles geometry
    sf::Texture m_tileset;      // Tileset image
};
