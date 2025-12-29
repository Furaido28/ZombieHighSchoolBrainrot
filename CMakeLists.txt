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
        src/views/sources/menu/MenuView.cpp
        src/controllers/headers/MenuController.h
        src/controllers/sources/MenuController.cpp
        src/scenes/sources/MenuScene.cpp
        src/core/headers/Scene.h
        src/core/headers/SceneManager.h
        src/core/sources/SceneManager.cpp
        src/models/headers/Player.h
        src/models/sources/Player.cpp
        src/models/headers/GameState.h
        src/models/sources/GameState.cpp
        src/views/headers/game/PlayerView.h
        src/views/sources/game/PlayerView.cpp
        src/controllers/headers/GameController.h
        src/controllers/sources/GameController.cpp
        src/scenes/headers/MenuScene.h
        src/scenes/headers/GameScene.h
        src/scenes/sources/GameScene.cpp
        src/models/sources/TileMap.cpp
        src/models/headers/TileMap.h
        src/views/sources/game/MapView.cpp
        src/views/headers/game/MapView.h
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
