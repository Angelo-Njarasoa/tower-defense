cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 REQUIRED graphics window system)

file(GLOB_RECURSE SOURCES
    src/main.cpp
    src/model/entity.cpp
    src/model/Enemy.cpp
    src/model/Wave.cpp
    src/model/Tower.cpp
    src/model/Map.cpp
)

add_executable(TowerDefense ${SOURCES})

target_include_directories(TowerDefense PRIVATE include)

target_link_libraries(TowerDefense sfml-graphics sfml-window sfml-system)
