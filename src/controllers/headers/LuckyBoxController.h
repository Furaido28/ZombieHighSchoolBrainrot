#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>

#include "models/headers/Item.h"
#include "models/headers/Player.h"
#include "core/headers/LuckyBoxSystem.h"
#include "core/headers/items/WorldItemSystem.h"

class LuckyBoxController {
public:
    LuckyBoxController(
        Player& player,
        WorldItemSystem& worldItemSystem,
        const std::map<std::string, sf::Texture>& itemTextures
    );

    void openLuckyBox(int itemIndex);

    // Screamer state
    bool isScreamerActive() const;
    void update(float dt);

    void render(sf::RenderWindow &window) const;

    void triggerScreamer();

private:
    Player& player;
    WorldItemSystem& worldItemSystem;
    const std::map<std::string, sf::Texture>& itemTextures;

    // Screamer
    bool screamerActive = false;
    float screamerTimer = 0.f;
    float screamerDuration = 0.6f;

    // Screamer visuals
    sf::Texture screamerTexture;
    sf::Sprite screamerSprite;


    sf::SoundBuffer screamerSoundBuffer;
    sf::Sound screamerSound;

    // Helpers
    Item makeMedkit() const;
    Item makePen() const;
    Item makeBook() const;
    Item makeChalk() const;
    Item makeLaptop() const;
    Item makeDeo() const;
};
