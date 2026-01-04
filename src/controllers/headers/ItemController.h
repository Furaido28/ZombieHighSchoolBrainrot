#pragma once

#include "models/headers/Player.h"
#include "core/headers/items/WorldItemSystem.h"

class GameController;

class ItemController {
public:
    ItemController(WorldItemSystem& worldItems, GameController& game);

    void tryPickup(Player& player);

    const sf::Texture &getTexture(const std::string &name) const;
private:
    WorldItemSystem& worldItemSystem;
    GameController& game;

    std::map<std::string, sf::Texture> itemTextures;

    void loadTextures();
};
