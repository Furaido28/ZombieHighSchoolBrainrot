#include "core/headers/commands/AttackCommand.h"

// Command that triggers a player attack
void AttackCommand::execute(float) {
    // Notify the player that an attack was requested
    player.requestAttack();
}
