#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "../patterns/Observer.hpp"
#include "../patterns/Factory.hpp"

// Forward declarations
class Game;
class Wave;
class Enemy;

//  Uses EnemyFactory (Factory Pattern) to instantiate enemies without scattered if/else chains, and notifies observers
public:

    //  Callback type fired on wave events
    //  (GameController subscribes to it to notify its observers)
    using EventCallback = std::function<void(GameEvent, int)>;

    //  Construction / Destruction
    /**
     * @brief Constructor
     * @param game           Shared pointer to the game model
     * @param totalWaves     Total number of waves in this session
     * @param prepTime       Preparation timer duration (seconds)
     * @param eventCallback  Callback invoked for each wave event
     */
    WaveController(std::shared_ptr<Game> game,
                   int                   totalWaves,
                   float                 prepTime,
                   EventCallback         eventCallback);

    ~WaveController() = default;

    WaveController(const WaveController&)            = delete;
    WaveController& operator=(const WaveController&) = delete;

    //  Update loop
    /**
     * @brief Updates the wave system
     * @param deltaTime  Time elapsed since the last frame (seconds)
     */
    void update(float deltaTime);

    //  Manual control
    /**
     * @brief Forces the next wave to start immediately
     *        (only works during the preparation phase)
     */
    void forceStartNextWave();

    //  Read-only accessors
    /** @return Current wave number (1-based) */
    int   getCurrentWaveNumber()  const;

    /** @return Total number of waves */
    int   getTotalWaves()         const;

    /** @return Time remaining before the next wave (seconds) */
    float getPrepTimeRemaining()  const;

    /** @return true if a wave is currently in progress */
    bool  isWaveInProgress()      const;

    /** @return true if all waves have been cleared */
    bool  allWavesCleared()       const;

private:

    //  Wave data
    std::shared_ptr<Game> m_game;
    int                   m_totalWaves;
    int                   m_currentWaveIndex = 0;   
    float                 m_prepDuration;            
    float                 m_prepTimer        = 0.f;  
    bool                  m_waveInProgress   = false;
    bool                  m_allCleared       = false;

    //  Progressive spawning
    float m_spawnInterval  = 1.0f; 
    float m_spawnTimer     = 0.f;  
    int   m_enemiesToSpawn = 0;    
    int   m_spawnedCount   = 0;     

    //  Callback to GameController
    EventCallback m_eventCallback;

    //  Private methods
    /**
     * @brief Starts the wave at the given index
     * @param waveIndex  0-based wave index
     */
    void startWave(int waveIndex);

    /**
     * @brief Spawns the next enemy of the current wave
     */
    void spawnNextEnemy();

    /**
     * @brief Checks whether the current wave has been fully cleared
     * @return true if all enemies are dead or have left the map
     */
    bool isCurrentWaveCleared() const;

    /**
     * @brief Determines which enemy type to spawn based on wave and spawn index
     * @param waveIndex   0-based wave index
     * @param spawnIndex  Spawn position within the wave
     * @return Enemy type to instantiate
     */
    EnemyType selectEnemyType(int waveIndex, int spawnIndex) const;

    /**
     * @brief Computes the number of enemies for a given wave (scaling difficulty)
     * @param waveIndex  0-based wave index
     * @return Number of enemies
     */
    int computeEnemyCount(int waveIndex) const;
};