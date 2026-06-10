#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "../patterns/Observer.hpp"
#include "../patterns/Factory.hpp"

class Game;

class WaveController {
public:
    using EventCallback = std::function<void(GameEvent, int)>;

    WaveController(std::shared_ptr<Game> game,
                   int                   totalWaves,
                   float                 prepTime,
                   EventCallback         eventCallback);
    ~WaveController() = default;
    WaveController(const WaveController&)            = delete;
    WaveController& operator=(const WaveController&) = delete;

    void  update(float deltaTime);
    void  forceStartNextWave();

    int   getCurrentWaveNumber() const;
    int   getTotalWaves()        const;
    float getPrepTimeRemaining() const;
    bool  isWaveInProgress()     const;
    bool  allWavesCleared()      const;

private:
    std::shared_ptr<Game> m_game;
    int   m_totalWaves;
    int   m_currentWaveIndex = 0;
    float m_prepDuration;
    float m_prepTimer        = 0.f;
    bool  m_waveInProgress   = false;
    bool  m_allCleared       = false;

    float m_spawnInterval  = 1.0f;
    float m_spawnTimer     = 0.f;
    int   m_enemiesToSpawn = 0;
    int   m_spawnedCount   = 0;

    EventCallback m_eventCallback;

    void      startWave(int waveIndex);
    void      spawnNextEnemy();
    bool      isCurrentWaveCleared() const;
    EnemyType selectEnemyType(int waveIndex, int spawnIndex) const;
    int       computeEnemyCount(int waveIndex) const;
};
