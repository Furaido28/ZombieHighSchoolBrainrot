#pragma once
#include <SFML/Graphics.hpp>
#include "../../models/TileMap.h"

class MapView : public sf::Drawable, public sf::Transformable {
public:
    bool load(const TileMap& map);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset; // <-- C'est ici qu'on va stocker l'image
};