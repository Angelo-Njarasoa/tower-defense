#include "../../include/model/Map.hpp"

Map::Map()
{
    return 0;
}

bool Map::load(const std::string& texturePath)
{
    if (!texture.loadFromFile(texturePath))
        return false;

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
    pathPoints.emplace_back(x, y);
}

void Map::addTowerSpot(float x, float y)
{
    towerSpots.emplace_back(x, y);
}
