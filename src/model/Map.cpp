#include "../../include/model/Map.hpp"
#include <iostream>

// 0 = herbe, 1 = chemin, 2 = base de tour
static const int GRID[MAP_ROWS][MAP_COLS] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
};

Map::Map() {}

bool Map::load(const std::string& grassPath, const std::string& pathPath, const std::string& basePath) {
    if (!m_texGrass.loadFromFile(grassPath)) {
        std::cerr << "Erreur chargement: " << grassPath << "\n";
        return false;
    }
    if (!m_texPath.loadFromFile(pathPath)) {
        std::cerr << "Erreur chargement: " << pathPath << "\n";
        return false;
    }
    if (!m_texBase.loadFromFile(basePath)) {
        std::cerr << "Erreur chargement: " << basePath << "\n";
        return false;
    }

    float half = TILE_SIZE / 2.f;
    m_waypoints = {
        {0.f,                  2 * TILE_SIZE + half},
        {4 * TILE_SIZE + half, 2 * TILE_SIZE + half},
        {4 * TILE_SIZE + half, 4 * TILE_SIZE + half},
        {7 * TILE_SIZE + half, 4 * TILE_SIZE + half},
        {7 * TILE_SIZE + half, 6 * TILE_SIZE + half},
        {12.f * TILE_SIZE,     6 * TILE_SIZE + half},
    };
    return true;
}

void Map::draw(sf::RenderWindow& window) {
    sf::Sprite sprite;
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            switch (GRID[row][col]) {
                case 1:  sprite.setTexture(m_texPath);  break;
                case 2:  sprite.setTexture(m_texBase);  break;
                default: sprite.setTexture(m_texGrass); break;
            }
            auto sz = sprite.getTexture()->getSize();
            sprite.setScale(TILE_SIZE / (float)sz.x, TILE_SIZE / (float)sz.y);
            sprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            window.draw(sprite);
        }
    }
}

const std::vector<sf::Vector2f>& Map::getWaypoints() const {
    return m_waypoints;
}
