#include "../../include/controller/WaveController.hpp"

// These includes will be available once the Model classes are implemented
// #include "../../include/model/Game.hpp"
// #include "../../include/model/Wave.hpp"
// #include "../../include/model/Enemy.hpp"

#include <iostream>
#include <stdexcept>

// ============================================================
//  Construction
// ============================================================

WaveController::WaveController(std::shared_ptr<Game> game,
                               int                   totalWaves,
                               float                 prepTime,
                               EventCallback         eventCallback)
    : m_game(std::move(game))
    , m_totalWaves(totalWaves)
    , m_prepDuration(prepTime)
    , m_prepTimer(prepTime)   // Start with a full timer before wave 1
    , m_eventCallback(std::move(eventCallback))
{
    if (m_totalWaves <= 0)
        throw std::invalid_argument("WaveController: totalWaves must be > 0");
}

// ============================================================
//  Update loop
// ============================================================

void WaveController::update(float deltaTime)
{
    if (m_allCleared) return;

    // ---- Preparation phase (between waves) ----
    if (!m_waveInProgress)
    {
        m_prepTimer -= deltaTime;

        if (m_prepTimer <= 0.f)
            startWave(m_currentWaveIndex);

        return;
    }

    // ---- Spawn phase ----
    if (m_enemiesToSpawn > 0)
    {
        m_spawnTimer += deltaTime;

        if (m_spawnTimer >= m_spawnInterval)
        {
            m_spawnTimer = 0.f;
            spawnNextEnemy();
        }
    }

    // ---- Update active enemies ----
    // m_game->getCurrentWave()->update(deltaTime);

    // ---- Wave cleared? ----
    if (m_enemiesToSpawn == 0 && isCurrentWaveCleared())
    {
        m_waveInProgress = false;

        // Callback to GameController -> notifies observers
        m_eventCallback(GameEvent::WAVE_CLEARED, m_currentWaveIndex + 1);

        ++m_currentWaveIndex;

        if (m_currentWaveIndex >= m_totalWaves)
        {
            m_allCleared = true;
            return;
        }

        // Reset the preparation timer for the next wave
        m_prepTimer = m_prepDuration;
    }
}

// ============================================================
//  Manual control
// ============================================================

void WaveController::forceStartNextWave()
{
    if (!m_waveInProgress && !m_allCleared)
    {
        m_prepTimer = 0.f;
        // The next call to update() will trigger startWave()
    }
}

// ============================================================
//  Accessors
// ============================================================

int WaveController::getCurrentWaveNumber() const
{
    return m_currentWaveIndex + 1;  // 1-based display
}

int WaveController::getTotalWaves() const
{
    return m_totalWaves;
}

float WaveController::getPrepTimeRemaining() const
{
    return m_waveInProgress ? 0.f : std::max(0.f, m_prepTimer);
}

bool WaveController::isWaveInProgress() const
{
    return m_waveInProgress;
}

bool WaveController::allWavesCleared() const
{
    return m_allCleared;
}

// ============================================================
//  Private methods
// ============================================================

void WaveController::startWave(int waveIndex)
{
    m_waveInProgress = true;
    m_spawnedCount   = 0;
    m_spawnTimer     = 0.f;
    m_enemiesToSpawn = computeEnemyCount(waveIndex);

    // Decrease spawn interval as waves progress (increasing difficulty)
    m_spawnInterval = std::max(0.3f, 1.0f - waveIndex * 0.05f);

    // Callback: announce the start of the wave (1-based number)
    m_eventCallback(GameEvent::WAVE_STARTED, waveIndex + 1);

    std::cout << "[WaveController] Wave " << (waveIndex + 1)
              << " — " << m_enemiesToSpawn << " enemies\n";
}

void WaveController::spawnNextEnemy()
{
    if (m_enemiesToSpawn <= 0) return;

    // Factory Pattern: select the enemy type based on wave and spawn position
    EnemyType type = selectEnemyType(m_currentWaveIndex, m_spawnedCount);

    // Create the enemy via EnemyFactory (unique_ptr ownership transferred to model)
    auto enemy = EnemyFactory::create(type, m_currentWaveIndex + 1);

    if (enemy)
    {
        // m_game->addEnemy(std::move(enemy));
        --m_enemiesToSpawn;
        ++m_spawnedCount;
    }
}

bool WaveController::isCurrentWaveCleared() const
{
    // Wave is cleared when no enemies remain active on the map
    // return m_game->getActiveEnemies().empty();

    // Temporary stub
    return false;
}

EnemyType WaveController::selectEnemyType(int waveIndex, int spawnIndex) const
{
    // Wave composition logic:
    //  - Waves 0-2 : Goblins only
    //  - Waves 3-6 : Goblins + Trolls (1 Troll every 3 spawns)
    //  - Waves 7+  : Goblins + Trolls + 1 Boss at the end of the wave

    if (waveIndex >= 7)
    {
        int total = computeEnemyCount(waveIndex);
        if (spawnIndex == total - 1)
            return EnemyType::BOSS;   // Last enemy = Boss
        if (spawnIndex % 3 == 2)
            return EnemyType::TROLL;
        return EnemyType::GOBLIN;
    }

    if (waveIndex >= 3)
    {
        if (spawnIndex % 3 == 2)
            return EnemyType::TROLL;
        return EnemyType::GOBLIN;
    }

    return EnemyType::GOBLIN;
}

int WaveController::computeEnemyCount(int waveIndex) const
{
    // Scaling difficulty formula:
    //   Wave 1 -> 5 enemies, +3 per wave
    return 5 + waveIndex * 3;
}