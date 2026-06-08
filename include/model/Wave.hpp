#pragma once
#include <vector>
#include <memory>
#include "../../include/model/Enemy.hpp"

class Wave {
private:
    int                                    m_waveNumber;
    std::vector<std::shared_ptr<Enemy>>    m_enemies;

public:
    Wave(int waveNumber);

    void addEnemy(std::shared_ptr<Enemy> enemy);
    std::vector<std::shared_ptr<Enemy>>& getEnemies();
    int  getWaveNumber() const;
    bool isCleared() const;
};
