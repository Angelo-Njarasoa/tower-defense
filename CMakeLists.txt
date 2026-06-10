cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 REQUIRED graphics window system audio)

set(SOURCES
    src/main.cpp
    src/model/entity.cpp
    src/model/Enemy.cpp
    src/model/Tower.cpp
    src/model/Map.cpp
    src/model/Wave.cpp
    src/model/Game.cpp
    src/patterns/Factory.cpp
    src/controller/WaveController.cpp
    src/controller/GameController.cpp
    src/view/GameView.cpp
    src/view/SoundManager.cpp
)

add_executable(TowerDefense ${SOURCES})

target_include_directories(TowerDefense PRIVATE include)

target_link_libraries(TowerDefense sfml-graphics sfml-window sfml-system sfml-audio)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
