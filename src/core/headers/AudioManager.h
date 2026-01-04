#pragma once
#include <SFML/Audio.hpp>
#include <algorithm>

// Singleton class to manage global audio settings (Music & SFX)
class AudioManager {
public:
    // Get the single instance of this class (Singleton pattern)
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    // Set Master volume (clamped between 0 and 100)
    void setGeneralVolume(float v) {
        generalVolume = std::clamp(v, 0.f, 100.f);
    }

    // Set Music volume
    void setMusicVolume(float v) {
        musicVolume = std::clamp(v, 0.f, 100.f);
    }

    // Set Sound Effects volume
    void setSoundEffectVolume(float v) {
        soundEffectVolume = std::clamp(v, 0.f, 100.f);
    }

    // Getters for current settings (for UI sliders)
    float getGeneralVolume() const { return generalVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getSoundEffectVolume() const { return soundEffectVolume; }

    // Calculate the actual volume (Master % * Music)
    float getFinalMusicVolume() const {
        return (generalVolume / 100.f) * musicVolume;
    }

    // Calculate the actual volume (Master % * SFX)
    float getFinalSFXVolume() const {
        return (generalVolume / 100.f) * soundEffectVolume;
    }

private:
    // Private constructor: Default volumes
    AudioManager()
        : generalVolume(100.f),
          musicVolume(50.f),
          soundEffectVolume(50.f) {}

    float generalVolume;       // Master volume
    float musicVolume;         // Background music
    float soundEffectVolume;   // Sound effects
};