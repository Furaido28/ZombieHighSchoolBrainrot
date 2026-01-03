#include <memory>
#include <stack>

#include "Scene.h"
#include "scenes/headers/MenuScene.h"

class SceneManager {
public:
    template<typename T, typename... Args>
    void pushScene(sf::RenderWindow* window, Args&&... args) {
        sceneStack.push(std::make_unique<T>(this, window, std::forward<Args>(args)...));
    }

    void popScene() {
        if (!sceneStack.empty())
            sceneStack.pop();
    }

    Scene* getCurrentScene() {
        return sceneStack.empty() ? nullptr : sceneStack.top().get();
    }

    void resetToMenu(sf::RenderWindow* window) {
        while (!sceneStack.empty())
            sceneStack.pop();

        pushScene<MenuScene>(window);
    }

private:
    std::stack<std::unique_ptr<Scene>> sceneStack;
};
