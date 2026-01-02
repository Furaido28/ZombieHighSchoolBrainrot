cmake_minimum_required(VERSION 3.20)

project(ZombieHighSchoolBrainrot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML
set(SFML_DIR "C:/libs/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Tous les .cpp du projet
file(GLOB_RECURSE SOURCES
        src/*.cpp
)

add_executable(ZombieHighSchoolBrainrot ${SOURCES}
        src/models/headers/Boss/phases/BossPhase.h
        src/models/headers/Boss/phases/BossPhaseNormal.h
        src/models/headers/Boss/phases/BossPhaseEnraged.h
        src/models/headers/Boss/phases/BossPhaseFrenzy.h
        src/models/sources/Boss/phases/BossPhaseNormal.cpp
        src/models/sources/Boss/phases/BossPhaseEnraged.cpp
        src/models/sources/Boss/phases/BossPhaseFrenzy.cpp)

target_include_directories(ZombieHighSchoolBrainrot PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ZombieHighSchoolBrainrot PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# Copier les assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})