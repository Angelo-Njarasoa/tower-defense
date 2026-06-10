#include "../../include/patterns/Factory.hpp"
#include "../../include/model/Tower.hpp"
#include "../../include/model/Enemy.hpp"
#include <stdexcept>

// TowerFactory
std::unique_ptr<Tower> TowerFactory::create(TowerType type, int gridX, int gridY) {
    return std::make_unique<Tower>(type, sf::Vector2i(gridX, gridY));
}

int TowerFactory::getCost(TowerType type) {
    switch (type) {
        case TowerType::GATLING: return 50;
        case TowerType::CANNON:  return 80;
        case TowerType::ROCKET:  return 120;
        default: throw std::invalid_argument("TowerFactory::getCost — unknown type");
    }
}

std::string TowerFactory::getName(TowerType type) {
    switch (type) {
        case TowerType::GATLING: return "Gatling";
        case TowerType::CANNON:  return "Cannon";
        case TowerType::ROCKET:  return "Rocket";
        default: throw std::invalid_argument("TowerFactory::getName — unknown type");
    }
}

std::string TowerFactory::getTexturePath(TowerType type) {
    switch (type) {
        case TowerType::GATLING: return "assets/tower_gatling.png";
        case TowerType::CANNON:  return "assets/tower_cannon.png";
        case TowerType::ROCKET:  return "assets/tower_rocket.png";
        default: throw std::invalid_argument("TowerFactory::getTexturePath — unknown type");
    }
}

// EnemyFactory
std::unique_ptr<Enemy> EnemyFactory::create(EnemyType type, int /*waveNumber*/,
                                             const std::vector<sf::Vector2f>& waypoints) {
    return std::make_unique<Enemy>(type, waypoints);
}

int EnemyFactory::getReward(EnemyType type) {
    switch (type) {
        case EnemyType::JEEP:  return 10;
        case EnemyType::TANK:  return 30;
        case EnemyType::PLANE: return 80;
        default: throw std::invalid_argument("EnemyFactory::getReward — unknown type");
    }
}

std::string EnemyFactory::getTexturePath(EnemyType type) {
    switch (type) {
        case EnemyType::JEEP:  return "assets/enemy_jeep.png";
        case EnemyType::TANK:  return "assets/enemy_tank.png";
        case EnemyType::PLANE: return "assets/enemy_plane.png";
        default: throw std::invalid_argument("EnemyFactory::getTexturePath — unknown type");
    }
}
