#pragma once
#include "BossPhase.h"
#include "models/headers/Boss/Boss.h"

class BossPhaseEnraged : public BossPhase {
public:
    void onEnter(Boss& boss) override {
        boss.setMoveSpeed(90.f);
    }

    void update(Boss& boss, float dt, const sf::Vector2f& playerPos) override {
        boss.chasePlayer(dt, playerPos);
    }

    int getDamage() const override {
        return 40;
    }
};
