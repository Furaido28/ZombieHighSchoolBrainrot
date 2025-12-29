cmake_minimum_required(VERSION 3.20)
set(CMAKE_BUILD_TYPE Release)
project(ZombieHighSchoolBrainrot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Chemin vers SFML (version MinGW ou VC, selon ton install)
set(SFML_DIR "C:/libs/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

file(GLOB_RECURSE SOURCES
        src/*.cpp
)

add_executable(ZombieHighSchoolBrainrot ${SOURCES})

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