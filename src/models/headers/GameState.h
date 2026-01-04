#pragma once

// Stores global game state information
class GameState {
public:
    // Player score
    int score = 0;

    // Indicates whether the game is currently paused
    bool isPaused = false;
};
