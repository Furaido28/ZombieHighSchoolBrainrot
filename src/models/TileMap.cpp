#include "TileMap.h"
#include <fstream>
#include <iostream>

bool TileMap::loadFromFile(const std::string& path, unsigned ts) {
    tileSize = ts;
    // On retire la configuration de tileShape.

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "TileMap: impossible d'ouvrir " << path << '\n';
        return false;
    }

    mapData.clear();
    std::string line;
    while (std::getline(file, line)) {
        // Nettoyage: gestion du retour chariot Windows/Linux
        if (!line.empty() && line.back() == '\r') line.pop_back();
        mapData.push_back(line);
    }
    file.close();

    height = mapData.size();
    width = height > 0 ? mapData[0].size() : 0;

    return true;
}

// LA MÉTHODE draw() EST SUPPRIMÉE DE CE FICHIER.

char TileMap::getTile(int x, int y) const {
    if (y < 0 || y >= (int)height || x < 0 || x >= (int)width) return '#';
    return mapData[y][x];
}