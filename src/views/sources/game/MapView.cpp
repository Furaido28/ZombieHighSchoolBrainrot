#include "../../headers/game/MapView.h"
#include <iostream>
#include <cstdlib> // Necessary for std::rand()

bool MapView::load(const TileMap& map) {
    if (!m_tileset.loadFromFile("assets/tileset.png")) {
        std::cerr << "ERROR: Cannot load assets/tileset.png" << std::endl;
        return false;
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(map.getWidth() * map.getHeight() * 4);

    // --- SIZE CONFIGURATION ---

    // 1. Logical Size (World): 64px
    float worldSize = (float)map.getTileSize();

    // 2. Texture Size (Image): 64px
    float textureSize = 64.0f;

    // Calculate number of columns in the image
    int tilesPerRow = m_tileset.getSize().x / (int)textureSize;

    for (unsigned i = 0; i < map.getWidth(); ++i) {
        for (unsigned j = 0; j < map.getHeight(); ++j) {

            char tileType = map.getTile(i, j);
            int tileIndex = 0;

            // --- MAPPING & TEXTURE SELECTION ---
            if (tileType == '.')      tileIndex = 0; // FLOOR
            else if (tileType == '#') tileIndex = 1; // WALL
            else if (tileType == 'B') tileIndex = 2; // BENCH
            else if (tileType == 'R') tileIndex = 3; // TABLE
            else if (tileType == 'N') tileIndex = 4; // BLACK
            else if (tileType == 'A') tileIndex = 5; // BLOODY BENCH
            else if (tileType == '*') tileIndex = 10; // Grass
            else if (tileType == 'T') tileIndex = 11; // Tree
            else if (tileType == 'r') tileIndex = 12; // Rock
            else if (tileType == '~') tileIndex = 13; // Water

            // --- RANDOM BLOOD MANAGEMENT ---
            else if (tileType == ',') {
                // 1. List blood image indexes in the tileset
                //    (Even if they're not next to each other)
                //    Example: if your images are at positions 6, 8 and 12.
                int bloodIndices[] = { 6, 7, 8, 9 };

                // 2. Count how many variants we have (here 4)
                int numberOfVariants = 4;

                // 3. Choose one randomly
                int randomIndex = std::rand() % numberOfVariants;
                tileIndex = bloodIndices[randomIndex];
            }
            // ----------------------------------

            else if (
                tileType == '$' ||
                tileType == '1' ||
                tileType == '2' ||
                tileType == '3'
                ) tileIndex = 0; // Items spawn on standard floor

            // Calculate column/row in the tileset
            int tu = tileIndex % tilesPerRow;
            int tv = tileIndex / tilesPerRow;

            sf::Vertex* quad = &m_vertices[(i + j * map.getWidth()) * 4];

            // --- A. POSITION ---
            quad[0].position = sf::Vector2f(i * worldSize, j * worldSize);
            quad[1].position = sf::Vector2f((i + 1) * worldSize, j * worldSize);
            quad[2].position = sf::Vector2f((i + 1) * worldSize, (j + 1) * worldSize);
            quad[3].position = sf::Vector2f(i * worldSize, (j + 1) * worldSize);

            // --- B. TEXTURE & ROTATION ---
            // Base coordinates in the image (according to chosen index above)
            float u = (float)(tu * (int)textureSize);
            float v = (float)(tv * (int)textureSize);

            // Define the 4 corners of the texture
            sf::Vector2f topLeft     = sf::Vector2f(u, v);
            sf::Vector2f topRight    = sf::Vector2f(u + textureSize, v);
            sf::Vector2f bottomRight = sf::Vector2f(u + textureSize, v + textureSize);
            sf::Vector2f bottomLeft  = sf::Vector2f(u, v + textureSize);

            // Default (Rotation 0°)
            sf::Vector2f t0 = topLeft;      // Top-Left
            sf::Vector2f t1 = topRight;     // Top-Right
            sf::Vector2f t2 = bottomRight;  // Bottom-Right
            sf::Vector2f t3 = bottomLeft;   // Bottom-Left

            // RANDOM ROTATION (Only for blood ',')
            // This adds to the image choice for even more variety!
            if (tileType == ',') {
                int rotation = std::rand() % 4; // 0, 1, 2 or 3

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

            // Apply texture coordinates
            quad[0].texCoords = t0;
            quad[1].texCoords = t1;
            quad[2].texCoords = t2;
            quad[3].texCoords = t3;

            // Color
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