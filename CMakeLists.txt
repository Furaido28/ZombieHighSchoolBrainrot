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

        # Menu
        src/views/sources/menu/MenuView.cpp
        src/controllers/sources/MenuController.cpp
        src/scenes/sources/MenuScene.cpp

        # Core
        src/core/sources/SceneManager.cpp

        # Player
        src/models/sources/Player.cpp
        src/models/sources/GameState.cpp
        src/views/sources/game/PlayerView.cpp

        # Game
        src/controllers/sources/GameController.cpp
        src/scenes/sources/GameScene.cpp

        # Map
        src/models/sources/TileMap.cpp
        src/views/sources/game/MapView.cpp

        # ENEMIES
        src/models/sources/Enemy.cpp
        src/models/sources/ZombieBasic.cpp
        src/models/sources/ZombieFast.cpp
        src/models/sources/ZombieTank.cpp
        src/models/sources/Boss.cpp

        # ENEMY VIEW
        src/views/sources/game/EnemyView.cpp
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
