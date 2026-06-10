#include "../../include/controller/WaveController.hpp"
#include "../../include/model/Game.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

WaveController::WaveController(std::shared_ptr<Game> game,
                               int                   totalWaves,
                               float                 prepTime,
                               EventCallback         eventCallback)
    : m_game(std::move(game))
    , m_totalWaves(totalWaves)
    , m_prepDuration(prepTime)
    , m_prepTimer(prepTime)
    , m_eventCallback(std::move(eventCallback))
{
    if (m_totalWaves <= 0)
        throw std::invalid_argument("WaveController: totalWaves must be > 0");
}

void WaveController::update(float deltaTime) {
    if (m_allCleared) return;

    // Preparation phase: countdown before next wave
    if (!m_waveInProgress) {
        m_prepTimer -= deltaTime;
        if (m_prepTimer <= 0.f)
            startWave(m_currentWaveIndex);
        return;
    }

    // Spawn phase: add one enemy every spawn interval
    if (m_enemiesToSpawn > 0) {
        m_spawnTimer += deltaTime;
        if (m_spawnTimer >= m_spawnInterval) {
            m_spawnTimer = 0.f;
            spawnNextEnemy();
        }
    }

    // Check if wave is fully cleared (all enemies dead or at base)
    if (m_enemiesToSpawn == 0 && isCurrentWaveCleared()) {
        m_waveInProgress = false;
        m_eventCallback(GameEvent::WAVE_CLEARED, m_currentWaveIndex + 1);
        ++m_currentWaveIndex;

        if (m_currentWaveIndex >= m_totalWaves) {
            m_allCleared = true;
            return;
        }
        m_prepTimer = m_prepDuration;
    }
}

void WaveController::forceStartNextWave() {
    if (!m_waveInProgress && !m_allCleared)
        m_prepTimer = 0.f;
}

int WaveController::getCurrentWaveNumber() const {
    return m_currentWaveIndex + 1;
}

int WaveController::getTotalWaves() const {
    return m_totalWaves;
}

float WaveController::getPrepTimeRemaining() const {
    return m_waveInProgress ? 0.f : std::max(0.f, m_prepTimer);
}

bool WaveController::isWaveInProgress() const {
    return m_waveInProgress;
}

bool WaveController::allWavesCleared() const {
    return m_allCleared;
}

void WaveController::startWave(int waveIndex) {
    m_waveInProgress = true;
    m_spawnedCount   = 0;
    m_spawnTimer     = 0.f;
    m_enemiesToSpawn = computeEnemyCount(waveIndex);
    // Spawn interval decreases with each wave (harder pacing)
    m_spawnInterval  = std::max(0.3f, 1.0f - waveIndex * 0.05f);
    m_game->currentWave = waveIndex + 1;
    m_eventCallback(GameEvent::WAVE_STARTED, waveIndex + 1);
    std::cout << "[Wave " << (waveIndex + 1) << "] " << m_enemiesToSpawn << " enemies\n";
}

void WaveController::spawnNextEnemy() {
    if (m_enemiesToSpawn <= 0) return;

    EnemyType type  = selectEnemyType(m_currentWaveIndex, m_spawnedCount);
    auto      enemy = EnemyFactory::create(type, m_currentWaveIndex + 1,
                                           m_game->map.getWaypoints());
    if (enemy) {
        enemy->loadTexture(EnemyFactory::getTexturePath(type));
        m_game->enemies.push_back(std::move(enemy));
        --m_enemiesToSpawn;
        ++m_spawnedCount;
    }
}

bool WaveController::isCurrentWaveCleared() const {
    for (const auto& e : m_game->enemies) {
        if (!e->isDead() && !e->hasReachedBase())
            return false;
    }
    return true;
}

EnemyType WaveController::selectEnemyType(int waveIndex, int spawnIndex) const {
    if (waveIndex >= 7) {
        int total = computeEnemyCount(waveIndex);
        if (spawnIndex == total - 1) return EnemyType::PLANE; // boss-tier enemy last
        if (spawnIndex % 3 == 2)     return EnemyType::TANK;
        return EnemyType::JEEP;
    }
    if (waveIndex >= 3) {
        if (spawnIndex % 3 == 2) return EnemyType::TANK;
        return EnemyType::JEEP;
    }
    return EnemyType::JEEP;
}

int WaveController::computeEnemyCount(int waveIndex) const {
    return 5 + waveIndex * 3; // Wave 1 = 5, +3 per wave
}
