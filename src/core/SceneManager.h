#pragma once
#include <memory>
#include "Scene.h"

class SceneManager {
public:
    SceneManager() = default;

    template<typename T, typename... Args>
    void changeScene(sf::RenderWindow* window, Args&&... args) {
        currentScene = std::make_unique<T>(this, window, std::forward<Args>(args)...);
    }

    Scene* getCurrentScene() {
        return currentScene.get();
    }

private:
    std::unique_ptr<Scene> currentScene;
};