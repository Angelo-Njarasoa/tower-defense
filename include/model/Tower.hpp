#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../../include/model/entity.hpp"
#include "../../include/model/Types.hpp"
#include "../../include/model/Enemy.hpp"

class Tower : public Entity {
private:
    sf::Texture  m_texture;
    TowerType    m_type;
    int          m_damage;
    float        m_range;
    float        m_fireRate;
    float        m_fireCooldown;
    float        m_flashTimer;      // Time remaining for the attack line visual
    sf::Vector2f m_lastTargetPos;   // Position of the last hit enemy
    int          m_cost;
    int          m_upgradeLevel;
    int          m_upgradeCost;
    sf::Vector2i m_gridPos;

public:
    Tower(TowerType type, sf::Vector2i gridPos);

    bool loadTexture(const std::string& path);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::shared_ptr<Enemy>>& enemies);
    void upgrade();
    bool canUpgrade() const;

    TowerType    getType()        const;
    int          getCost()        const;
    int          getUpgradeCost() const;
    float        getRange()       const;
    sf::Vector2f getPosition()    const;
    sf::Vector2i getGridPosition()const;
};
