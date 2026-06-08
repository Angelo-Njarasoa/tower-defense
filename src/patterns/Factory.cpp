#include "../../include/patterns/Factory.hpp"

#include <stdexcept>
#include <string>

//  TowerFactory — Implementation
std::unique_ptr<Tower> TowerFactory::create(TowerType type, int gridX, int gridY)
{
    switch (type)
    {
        case TowerType::ARCHER:
            break;

        case TowerType::MAGE:
            break;

        case TowerType::CANNON:
            break;

        default:
            throw std::invalid_argument("TowerFactory::create — unknown tower type");
    }
    return nullptr;
}

int TowerFactory::getCost(TowerType type)
{
    switch (type)
    {
        case TowerType::ARCHER: return 100;
        case TowerType::MAGE:   return 150;
        case TowerType::CANNON: return 200;
        default:
            throw std::invalid_argument("TowerFactory::getCost — unknown type");
    }
}

std::string TowerFactory::getName(TowerType type)
{
    switch (type)
    {
        case TowerType::ARCHER: return "Archer";
        case TowerType::MAGE:   return "Mage";
        case TowerType::CANNON: return "Cannon";
        default:
            throw std::invalid_argument("TowerFactory::getName — unknown type");
    }
}

//  EnemyFactory — Implementation
std::unique_ptr<Enemy> EnemyFactory::create(EnemyType type, int waveNumber)
{
    switch (type)
    {
        case EnemyType::GOBLIN:
            break;

        case EnemyType::TROLL:
            break;

        case EnemyType::BOSS:
            break;

        default:
            throw std::invalid_argument("EnemyFactory::create — unknown enemy type");
    }

    return nullptr;
}

int EnemyFactory::getReward(EnemyType type)
{
    switch (type)
    {
        case EnemyType::GOBLIN: return 10;
        case EnemyType::TROLL:  return 25;
        case EnemyType::BOSS:   return 100;
        default:
            throw std::invalid_argument("EnemyFactory::getReward — unknown type");
    }
}