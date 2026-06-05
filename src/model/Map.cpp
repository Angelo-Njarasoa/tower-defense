#include "../../include/model/Map.hpp"
#include <iostream>

Map::Map()
{
}

bool Map::load(const std::string& texturePath)
{
    if (!texture.loadFromFile(texturePath))
    {
        std::cerr << "Erreur : impossible de charger "
                  << texturePath << std::endl;
        return false;
    }

    sprite.setTexture(texture);
    return true;
}

void Map::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

const std::vector<sf::Vector2f>& Map::getPathPoints() const
{
    return pathPoints;
}

const std::vector<sf::Vector2f>& Map::getTowerSpots() const
{
    return towerSpots;
}

void Map::addPathPoint(float x, float y)
{
    pathPoints.push_back(sf::Vector2f(x, y));
}

void Map::addTowerSpot(float x, float y)
{
    towerSpots.push_back(sf::Vector2f(x, y));
}