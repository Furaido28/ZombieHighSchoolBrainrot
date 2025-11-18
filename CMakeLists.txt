cmake_minimum_required(VERSION 3.20)
project(ZombieHighSchoolBrainrot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/libs/SFML-2.6.1/lib/cmake/SFML")  # adapte ce chemin

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

add_executable(ZombieHighSchoolBrainrot
        src/main.cpp
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