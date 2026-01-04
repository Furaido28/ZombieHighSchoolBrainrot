#include "../../headers/game/EnemyView.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// ======================================================
// INTELLIGENT LOADING (GRID + SPECIFIC FRAMES)
// ======================================================
void EnemyView::loadDirectionalTextures(EnemyType type, const std::string& folder, const std::string& filePrefix,
                                        int fFront, int fBack, int fLeft, int fRight, float speed, int cols)
{
    EnemyAnimations anims;
    std::string basePath = "assets/animation/Enemy/" + folder + "/" + filePrefix + "_walking_";
    bool ok = true;

    auto load = [&](DirectionData& data, const std::string& suffix, int count) {
        if (data.texture.loadFromFile(basePath + suffix)) {
            data.frameCount = count;
            data.cols = cols;
            return true;
        }
        return false;
    };

    // Load textures for all directions
    if (!load(anims.front, "front.png", fFront)) ok = false;
    if (!load(anims.back, "back.png", fBack)) ok = false;
    if (!load(anims.left, "left.png", fLeft)) ok = false;
    if (!load(anims.right, "right.png", fRight)) ok = false;

    if (ok) {
        anims.frameDuration = speed;
        anims.isLoaded = true;
        animationsMap[type] = anims;
        std::cout << "Animations loaded for " << folder << " (Cols: " << cols << ")\n";
    } else {
        std::cerr << "ERROR: Failed to load animations for " << folder << "\n";
    }
}

// ======================================================
// CONSTRUCTOR
// ======================================================
EnemyView::EnemyView()
{
    // --------------------------------------------------
    // 1. ANIMATED ENEMIES CONFIGURATION
    // --------------------------------------------------
    // BASIC enemy
    loadDirectionalTextures(EnemyType::Basic, "Basic", "basic_enemy", 33, 29, 33, 33, 0.05f, 6);
    // FAST enemy
    loadDirectionalTextures(EnemyType::Fast, "Fast", "fast_enemy", 23, 23, 24, 34, 0.04f, 5);
    // TANK enemy
    loadDirectionalTextures(EnemyType::Tank, "Tank", "tank_enemy", 31, 33, 50, 39, 0.05f, 6);

    // --------------------------------------------------
    // 2. BOSS LOADING
    // --------------------------------------------------
    if (!boss01Texture.loadFromFile("assets/animation/Boss/TralaleroTralala/idle.png")) std::cerr << "Error Boss01 idle\n";
    if (!boss02Texture.loadFromFile("assets/animation/Boss/ChimpanziniBananini/idle.png")) std::cerr << "Error Boss02 idle\n";
    if (!boss03Texture.loadFromFile("assets/animation/Boss/UdinDinDinDun/idle.png")) std::cerr << "Error Boss03 idle\n";
    if (!finalBossTexture.loadFromFile("assets/animation/Boss/OscarTheCrackhead/idle.png")) std::cerr << "Error FinalBoss idle\n";

    // --------------------------------------------------
    // 3. UI INITIALIZATION
    // --------------------------------------------------
    // Health bar background setup
    hbBack.setSize({ hpWidth - 2 * hpRadius, hpHeight });
    hbBack.setFillColor(sf::Color(30, 30, 30, 220));
    hbBackLeft.setRadius(hpRadius); hbBackRight.setRadius(hpRadius);
    hbBackLeft.setFillColor(hbBack.getFillColor()); hbBackRight.setFillColor(hbBack.getFillColor());

    // Health bar foreground setup
    hbFront.setFillColor(sf::Color::Green);
    hbFrontLeft.setRadius(hpRadius); hbFrontRight.setRadius(hpRadius);
    hbFrontLeft.setFillColor(sf::Color::Green); hbFrontRight.setFillColor(sf::Color::Green);

    // Add outlines
    hbBack.setOutlineThickness(1.f); hbBack.setOutlineColor(sf::Color::Black);
    hbBackLeft.setOutlineThickness(1.f); hbBackRight.setOutlineThickness(1.f);
    hbBackLeft.setOutlineColor(sf::Color::Black); hbBackRight.setOutlineColor(sf::Color::Black);
}

// ======================================================
// LOAD TEXTURE (BOSS ONLY)
// ======================================================
void EnemyView::loadTextureForType(EnemyType type)
{
    if (textureInitialized && type == currentType) return;

    currentType = type;
    textureInitialized = true;

    // Only Boss enemies are managed here now
    switch (type) {
        case EnemyType::Boss01:    sprite.setTexture(boss01Texture);      break;
        case EnemyType::Boss02:    sprite.setTexture(boss02Texture);      break;
        case EnemyType::Boss03:    sprite.setTexture(boss03Texture);      break;
        case EnemyType::FinalBoss: sprite.setTexture(finalBossTexture);   break;
        default: break;
    }

    if (sprite.getTexture()) {
        frameWidth  = sprite.getTexture()->getSize().x / 4;
        frameHeight = sprite.getTexture()->getSize().y;
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    }
}

// ======================================================
// DRAW HEALTH BAR
// ======================================================
void EnemyView::drawHealthBar(sf::RenderWindow& window, const Enemy& enemy)
{
    float maxHp = enemy.getMaxHealth();
    if (maxHp <= 0.f) return;

    float ratio = std::clamp(enemy.getHealth() / maxHp, 0.f, 1.f);
    sf::Vector2f basePos = enemy.getPosition();
    float yOffset = enemy.getRadius() + 10.f;
    sf::Vector2f pos(basePos.x - hpWidth / 2.f, basePos.y - yOffset);

    // Set background health bar positions
    hbBack.setPosition(pos.x + hpRadius, pos.y);
    hbBackLeft.setPosition(pos.x, pos.y);
    hbBackRight.setPosition(pos.x + hpWidth - 2 * hpRadius, pos.y);

    // Set foreground health bar positions
    float fillWidth = (hpWidth - 2 * hpRadius) * ratio;
    hbFront.setSize({ fillWidth, hpHeight });
    hbFront.setPosition(pos.x + hpRadius, pos.y);
    hbFrontLeft.setPosition(pos.x, pos.y);

    if (fillWidth > hpRadius) {
        hbFrontRight.setPosition(pos.x + hpRadius + fillWidth - hpRadius, pos.y);
    }

    // Health bar color based on health percentage
    sf::Color color;
    if (ratio > 0.6f) color = sf::Color(80, 220, 100);
    else if (ratio > 0.3f) color = sf::Color(240, 200, 80);
    else color = sf::Color(220, 80, 80);

    hbFront.setFillColor(color); hbFrontLeft.setFillColor(color); hbFrontRight.setFillColor(color);

    // Draw background health bar
    window.draw(hbBackLeft); window.draw(hbBack); window.draw(hbBackRight);
    
    // Draw foreground health bar if there's health left
    if (ratio > 0.f) {
        window.draw(hbFrontLeft); window.draw(hbFront);
        if (fillWidth > hpRadius) window.draw(hbFrontRight);
    }
}

// ======================================================
// RENDER (COMPLETE LOGIC)
// ======================================================
void EnemyView::render(sf::RenderWindow& window, const Enemy& enemy, const sf::Vector2f& playerPos)
{
    EnemyType type = enemy.getType();

    // =========================================================
    // CASE 1: ANIMATED ENEMIES (BASIC, FAST, TANK)
    // =========================================================
    if (type == EnemyType::Basic ||
        type == EnemyType::Fast  ||
        type == EnemyType::Tank)
    {
        EnemyAnimations& anims = animationsMap[type];
        sf::Vector2f v = enemy.getVelocity();

        bool isMoving = (v.x * v.x + v.y * v.y) > 1.0f;

        DirectionData* dirData = &anims.front;

        // Determine direction based on velocity
        if (std::abs(v.x) > std::abs(v.y)) {
            dirData = (v.x > 0) ? &anims.right : &anims.left;
        } else {
            if (v.y > 0) dirData = &anims.front;
            else if (v.y < 0) dirData = &anims.back;
        }

        // Calculate animation frame
        int frameIndex = 0;
        if (isMoving && dirData->frameCount > 1) {
            float time = globalClock.getElapsedTime().asSeconds();
            frameIndex = (int)(time / anims.frameDuration) % dirData->frameCount;
        }

        sprite.setTexture(dirData->texture);

        // Calculate texture rectangle
        frameWidth  = dirData->texture.getSize().x / dirData->cols;
        int rows    = (dirData->frameCount + dirData->cols - 1) / dirData->cols;
        frameHeight = dirData->texture.getSize().y / std::max(1, rows);

        int col = frameIndex % dirData->cols;
        int row = frameIndex / dirData->cols;

        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
        sprite.setTextureRect({ col * frameWidth, row * frameHeight, frameWidth, frameHeight });
    }
    // =========================================================
    // CASE 2: BOSS (FIXED)
    // =========================================================
    else
    {
        // Reset texture rectangle to avoid previous inheritance
        sprite.setTextureRect(sf::IntRect());

        // Force boss texture
        const sf::Texture* tex = nullptr;
        switch (type) {
            case EnemyType::Boss01:    tex = &boss01Texture; break;
            case EnemyType::Boss02:    tex = &boss02Texture; break;
            case EnemyType::Boss03:    tex = &boss03Texture; break;
            case EnemyType::FinalBoss: tex = &finalBossTexture; break;
            default: break;
        }

        if (tex) {
            sprite.setTexture(*tex, true);

            // 🔥 MANDATORY RECALCULATION
            frameWidth  = tex->getSize().x / 4;
            frameHeight = tex->getSize().y;
            sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
        }

        // Direction calculation for boss
        sf::Vector2f v = enemy.getVelocity();
        int dirIndex = 0;

        if (std::abs(v.x) > std::abs(v.y))
            dirIndex = (v.x < 0.f) ? 1 : 2;
        else
            dirIndex = (v.y < 0.f) ? 3 : 0;

        sprite.setTextureRect({
            dirIndex * frameWidth,
            0,
            frameWidth,
            frameHeight
        });
    }

    // =========================================================
    // POSITION & SCALE
    // =========================================================
    sprite.setPosition(enemy.getPosition());

    float scale = 1.0f;
    if (frameWidth > 0)
        scale = (enemy.getRadius() * 2.f) / (float)frameWidth;

    sprite.setScale(scale, scale);
    window.draw(sprite);

    // =========================================================
    // HEALTH BAR (only if close to player)
    // =========================================================
    sf::Vector2f d = enemy.getPosition() - playerPos;
    if ((d.x * d.x + d.y * d.y) < (150.f * 150.f) && enemy.isAlive())
        drawHealthBar(window, enemy);
}