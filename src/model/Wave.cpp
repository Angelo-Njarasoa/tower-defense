#include "../../include/model/Wave.hpp"

Wave::Wave(int waveNumber) : m_waveNumber(waveNumber) {}

void Wave::addEnemy(std::shared_ptr<Enemy> enemy) {
    m_enemies.push_back(enemy);
}

std::vector<std::shared_ptr<Enemy>>& Wave::getEnemies() {
    return m_enemies;
}

int Wave::getWaveNumber() const {
    return m_waveNumber;
}

// Wave is cleared when all enemies are dead or have reached the base
bool Wave::isCleared() const {
    for (const auto& enemy : m_enemies) {
        if (!enemy->isDead() && !enemy->hasReachedBase())
            return false;
    }
    return true;
}
