#include "core/headers/commands/AttackCommand.h"

void AttackCommand::execute(float) {
    player.requestAttack();
}