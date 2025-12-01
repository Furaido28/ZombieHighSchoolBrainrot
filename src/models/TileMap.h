#ifndef TILEMAP_MODEL_H
#define TILEMAP_MODEL_H

#include <vector>
#include <string>

class TileMap {
public:
    TileMap();
    bool loadFromFile(const std::string& filePath);

    // Getters pour la Vue et le Contrôleur
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getTileSize() const { return m_tileSize; }
    int getTile(int x, int y) const;

private:
    int m_width;
    int m_height;
    int m_tileSize;
    std::vector<int> m_tiles; // 0 = Sol (.), 1 = Mur (#)
};

#endif