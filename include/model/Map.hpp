#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

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
    bool isGrass(int col, int row) const;
};
