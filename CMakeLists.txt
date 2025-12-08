cmake_minimum_required(VERSION 3.20)
set(CMAKE_BUILD_TYPE Release)
project(ZombieHighSchoolBrainrot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Chemin vers SFML (version MinGW ou VC, selon ton install)
set(SFML_DIR "C:/libs/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

add_executable(ZombieHighSchoolBrainrot
        src/main.cpp
        src/views/menu/menu.cpp
        src/controllers/MenuController.h
        src/controllers/MenuController.cpp
        src/scenes/MenuScene.cpp
        src/core/Scene.h
        src/core/SceneManager.h
        src/core/SceneManager.cpp
        src/models/Player.h
        src/models/Player.cpp
        src/models/GameState.h
        src/models/GameState.cpp
        src/views/game/PlayerView.h
        src/views/game/PlayerView.cpp
        src/controllers/GameController.h
        src/controllers/GameController.cpp
        src/scenes/MenuScene.h
        src/scenes/GameScene.h
        src/scenes/GameScene.cpp
        src/models/TileMap.cpp
        src/models/TileMap.h
        src/views/game/MapView.cpp
        src/views/game/MapView.h
        src/Constants.h
)

target_include_directories(ZombieHighSchoolBrainrot PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ZombieHighSchoolBrainrot PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# 🔥 copier automatiquement le dossier assets vers le dossier de build
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
