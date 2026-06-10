#pragma once
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../model/Types.hpp"

class Tower;
class Enemy;

class TowerFactory {
public:
    TowerFactory() = delete;
    static std::unique_ptr<Tower> create(TowerType type, int gridX, int gridY);
    static int         getCost(TowerType type);
    static std::string getName(TowerType type);
    static std::string getTexturePath(TowerType type);
};

class EnemyFactory {
public:
    EnemyFactory() = delete;
    static std::unique_ptr<Enemy> create(EnemyType type, int waveNumber,
                                          const std::vector<sf::Vector2f>& waypoints);
    static int         getReward(EnemyType type);
    static std::string getTexturePath(EnemyType type);
};
