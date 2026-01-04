#include "../headers/TileMap.h"
#include <fstream>
#include <iostream>

// Loads the tile map from a text file
bool TileMap::loadFromFile(const std::string& path, unsigned ts) {
    // Store the tile size
    tileSize = ts;

    // Open the map file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "TileMap: impossible to open " << path << '\n';
        return false;
    }

    // Clear previous map data
    mapData.clear();

    std::string line;
    while (std::getline(file, line)) {
        // Clean line ending (Windows vs Linux compatibility)
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // Each line represents one row of the map
        mapData.push_back(line);
    }
    file.close();

    // Compute map dimensions
    height = mapData.size();
    width = height > 0 ? mapData[0].size() : 0;

    return true;
}

// Returns the tile character at the given grid position
char TileMap::getTile(int x, int y) const {
    // Out-of-bounds positions are treated as walls
    if (y < 0 || y >= (int)height || x < 0 || x >= (int)width)
        return '#';

    return mapData[y][x];
}
