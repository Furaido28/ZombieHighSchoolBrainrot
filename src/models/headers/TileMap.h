#pragma once

#include <string>
#include <vector>

// Represents a tile-based map loaded from a text file
class TileMap {
public:
    // Loads the map from a file and sets the tile size
    bool loadFromFile(const std::string& path, unsigned ts);

    // Returns the tile character at the given grid position
    char getTile(int x, int y) const;

    // Getters used by the View and the Controller
    unsigned getTileSize() const { return tileSize; }
    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }

private:
    // Raw map data (each string is one row)
    std::vector<std::string> mapData;

    // Size of one tile (in pixels)
    unsigned tileSize;

    // Map dimensions (in tiles)
    unsigned width;
    unsigned height;
};
