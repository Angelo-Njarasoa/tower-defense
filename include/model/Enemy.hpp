#pragma once
#include "../../include/model/entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class EnemyType { JEEP, TANK, PLANE };

class Enemy : public Entity
{
private:
    sf::Texture                m_texture;
    EnemyType                  m_type;
    float                      m_speed;
    int                        m_reward;
    int                        m_damage;
    std::vector<sf::Vector2f>  m_path;        // Waypoints the enemy follows
    int                        m_pathIndex;   // Current waypoint index
    float                      m_spawnDelay;  // Seconds before the enemy starts moving

public:
    Enemy(EnemyType type, std::vector<sf::Vector2f> path);

    bool loadTexture(const std::string& path);

    void      update(float deltaTime) override;
    void      render(sf::RenderWindow& window) const override;

    void      setSpawnDelay(float seconds);

    bool      hasReachedBase() const;
    bool      isDead()         const;
    int       getReward()      const;
    EnemyType getType()        const;
    sf::Vector2f getPosition() const;
};
