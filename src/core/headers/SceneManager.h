#pragma once

#include <memory>
#include <stack>

#include "Scene.h"
#include "scenes/headers/MenuScene.h"

// Manages the flow of the game (switching between Menu, Game, Options, etc.)
class SceneManager {
public:
    // Template function to add any type of Scene to the stack
    template<typename T, typename... Args>
    void pushScene(sf::RenderWindow* window, Args&&... args) {
        // Create the new scene and put it on top of the stack
        sceneStack.push(std::make_unique<T>(this, window, std::forward<Args>(args)...));
    }

    // Remove the current scene (Go back to previous screen)
    void popScene() {
        if (!sceneStack.empty())
            sceneStack.pop();
    }

    // Get the scene that is currently active (on top)
    Scene* getCurrentScene() {
        return sceneStack.empty() ? nullptr : sceneStack.top().get();
    }

    // Clear everything and go back to the Main Menu
    void resetToMenu(sf::RenderWindow* window) {
        // Remove all scenes
        while (!sceneStack.empty())
            sceneStack.pop();

        // Start with the Menu
        pushScene<MenuScene>(window);
    }

private:
    // The last scene added is the one currently shown
    std::stack<std::unique_ptr<Scene>> sceneStack;
};