#pragma once
#include "BossPhase.h"
#include "models/headers/Boss/Boss.h"

class BossPhaseFrenzy : public BossPhase {
public:
    void onEnter(Boss& boss) override {
        boss.setMoveSpeed(160.f);
    }

    void update(Boss& boss, float dt, const sf::Vector2f& playerPos) override {
        boss.chasePlayer(dt, playerPos);
    }

    int getDamage() const override {
        return 60;
    }
};
