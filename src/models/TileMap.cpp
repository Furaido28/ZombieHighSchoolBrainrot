#include "TileMap.h"
#include <fstream>
#include <iostream>

TileMap::TileMap() : m_width(0), m_height(0), m_tileSize(32) {}

bool TileMap::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible de charger la carte " << filePath << std::endl;
        return false;
    }

    m_tiles.clear();
    m_width = 0;
    m_height = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (m_width == 0) m_width = line.length();

        for (char c : line) {
            if (c == '#') m_tiles.push_back(1);      // Mur
            else if (c == '.') m_tiles.push_back(0); // Sol
            else m_tiles.push_back(0);               // Par défaut
        }
        m_height++;
    }
    return true;
}

int TileMap::getTile(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
        return m_tiles[x + y * m_width];
    return 1; // Collision par défaut si hors map
}