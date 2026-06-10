#pragma once
#include <vector>
#include <memory>
#include <set>
#include <utility>
#include "Map.hpp"
#include "Tower.hpp"
#include "Enemy.hpp"

class Game {
public:
    Map map;
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::set<std::pair<int,int>>        occupiedTiles;
    int gold        = 150;
    int lives       = 10;
    int score       = 0;
    int currentWave = 0;

    bool load();
};
