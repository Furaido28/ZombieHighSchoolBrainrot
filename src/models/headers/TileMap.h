#pragma once

#include <string>
#include <vector>

class TileMap {
public:
    bool loadFromFile(const std::string& path, unsigned ts);
    char getTile(int x, int y) const;

    // Getters pour la View et le Controller
    unsigned getTileSize() const { return tileSize; }
    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }

private:
    std::vector<std::string> mapData;
    unsigned tileSize;
    unsigned width;
    unsigned height;

    // sf::RectangleShape tileShape a été retiré.
};