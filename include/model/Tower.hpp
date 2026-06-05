#pragma once
#include <vector>
#include <memory>
#include "../../include/model/entity.hpp"
#include "../../include/model/Enemy.hpp"

enum class TowerType { ARCHER, MAGE, CANNON };

class Tower : public Entity {
private:
    TowerType    m_type;
    int          m_damage;
    float        m_range;
    float        m_fireRate;
    float        m_fireCooldown;
    int          m_cost;
    int          m_upgradeLevel;
    int          m_upgradeCost;
    sf::Vector2i m_gridPos;

public:
    Tower(TowerType type, sf::Vector2i gridPos);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::shared_ptr<Enemy>>& enemies);
    void upgrade();
    bool canUpgrade() const;

    TowerType    getType() const;
    int          getCost() const;
    int          getUpgradeCost() const;
    float        getRange() const;
    sf::Vector2i getGridPosition() const;
};
