#ifndef MAP_HPP
#define MAP_HPP

#pragma once
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
constexpr int   TILE_SIZE = 64;
constexpr int   MAP_COLS  = 12;
constexpr int   MAP_ROWS  = 9;

class Map {
private:
    sf::Texture m_texGrass;
    sf::Texture m_texPath;
    sf::Texture m_texBase;
    std::vector<sf::Vector2f> m_waypoints;

public:
    Map();
    bool load(const std::string& grassPath, const std::string& pathPath, const std::string& basePath);
    void draw(sf::RenderWindow& window);
    const std::vector<sf::Vector2f>& getWaypoints() const;
};
