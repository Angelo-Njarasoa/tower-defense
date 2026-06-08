#pragma once

#include <memory>
#include <string>
#include <stdexcept>

class Tower;
class Enemy;

//  Available type enumerations
enum class TowerType {
    ARCHER,  // Long range, low damage, fast fire rate
    MAGE,    // Medium range, magic damage (slows enemies)
    CANNON   // Short range, heavy damage, slow fire rate
};

enum class EnemyType {
    GOBLIN,  // Fast, low HP, low reward
    TROLL,   // Slow, high HP, medium reward
    BOSS     // Very slow, massive HP, high reward
};

//  TowerFactory — Factory Pattern for towers
class TowerFactory {
public:
    TowerFactory() = delete; 

    /**
     * @brief Creates a tower of the given type
     * @param type    Desired tower type
     * @param gridX   X position on the grid
     * @param gridY   Y position on the grid
     * @return
     * @throws
     */
    static std::unique_ptr<Tower> create(TowerType type, int gridX, int gridY);

    /**
     * @brief Returns the gold cost for a given tower type
     * @param type  Tower type
     * @return Cost in gold
     */
    static int getCost(TowerType type);

    /**
     * @brief Returns the display name for a given tower type
     * @param type  Tower type
     * @return Human readable name
     */
    static std::string getName(TowerType type);
};

//  Creates enemies based on the current wave
class EnemyFactory {
public:
    EnemyFactory() = delete;

    /**
     * @brief Creates an enemy of the given type
     * @param type          Desired enemy type
     * @param waveNumber    Current wave number (
     * @return 
     * @throws 
     */
    static std::unique_ptr<Enemy> create(EnemyType type, int waveNumber);

    /**
     * @brief Returns the gold reward for killing a given enemy type
     * @param type  Enemy type
     * @return Reward in gold
     */
    static int getReward(EnemyType type);
};