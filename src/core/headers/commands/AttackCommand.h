#pragma once

#include "../Command.h"
#include "models/headers/Player.h"
#include "models/headers/Enemy.h"
#include <vector>
#include <memory>

class AttackCommand : public Command {
public:
    AttackCommand(Player& player,
                  std::vector<std::unique_ptr<Enemy>>& enemies,
                  float& attackTimer,
                  float attackCooldown)
        : player(player),
          enemies(enemies),
          attackTimer(attackTimer),
          attackCooldown(attackCooldown) {}

    void execute(float dt) override;

private:
    Player& player;
    std::vector<std::unique_ptr<Enemy>>& enemies;
    float& attackTimer;
    float attackCooldown;
};
