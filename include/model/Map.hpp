#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    std::vector<sf::Vector2f> pathPoints;
    std::vector<sf::Vector2f> towerSpots;

public:
    Map();

    bool load(const std::string& texturePath);

    void draw(sf::RenderWindow& window);

    const std::vector<sf::Vector2f>& getPathPoints() const;
    const std::vector<sf::Vector2f>& getTowerSpots() const;

    void addPathPoint(float x, float y);
    void addTowerSpot(float x, float y);
};

#endif