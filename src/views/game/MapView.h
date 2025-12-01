#ifndef MAPVIEW_H
#define MAPVIEW_H
#include <SFML/Graphics.hpp>
#include "../../models/TileMap.h"

class MapView : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string& tilesetPath, const TileMap& model);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif