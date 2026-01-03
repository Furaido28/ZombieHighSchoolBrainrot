#pragma once
#include <SFML/Audio.hpp>
#include <algorithm>

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    void setGeneralVolume(float v) {
        generalVolume = std::clamp(v, 0.f, 100.f);
    }

    void setMusicVolume(float v) {
        musicVolume = std::clamp(v, 0.f, 100.f);
    }

    void setSoundEffectVolume(float v) {
        soundEffectVolume = std::clamp(v, 0.f, 100.f);
    }

    float getGeneralVolume() const { return generalVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getSoundEffectVolume() const { return soundEffectVolume; }

    // Volumes finaux pour SFML
    float getFinalMusicVolume() const {
        return (generalVolume / 100.f) * musicVolume;
    }

    float getFinalSFXVolume() const {
        return (generalVolume / 100.f) * soundEffectVolume;
    }

private:
    AudioManager()
        : generalVolume(100.f),
          musicVolume(50.f),
          soundEffectVolume(50.f) {}

    float generalVolume;
    float musicVolume;
    float soundEffectVolume;
};
