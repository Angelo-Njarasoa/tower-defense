#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "model/Map.hpp"

class Game
{
private:
    sf::RenderWindow window;
    sf::Event event;

    bool isRunning;

    Map map;

    void processEvents();
    void update();
    void render();

public:
    Game();
    ~Game();

    void run();
};

#endif