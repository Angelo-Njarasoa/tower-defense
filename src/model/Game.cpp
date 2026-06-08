#include "Game.hpp"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(800, 600), "Tower Defense")
{
    window.setFramerateLimit(60);

    if (!map.load("assets/map.png"))
    {
        std::cerr << "Erreur : impossible de charger la map" << std::endl;
    }

    map.addPathPoint(100.f, 100.f);
    map.addPathPoint(300.f, 100.f);
    map.addPathPoint(300.f, 300.f);
    map.addPathPoint(500.f, 300.f);

    map.addTowerSpot(200.f, 200.f);
    map.addTowerSpot(400.f, 250.f);
}