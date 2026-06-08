#include "../../include/model/Enemy.hpp"
#include <cmath>
#include <iostream>

// Sets stats based on enemy type; hp starts at full health
Enemy::Enemy(EnemyType type, std::vector<sf::Vector2f> path)
    : m_type(type), m_path(path), m_pathIndex(0), m_spawnDelay(0.f)
{
    switch (type) {
        case EnemyType::JEEP: // Fast scout vehicle, low HP
            m_hpMax  = 80;
            m_speed  = 110.f;
            m_reward = 10;
            m_damage = 10;
            break;
        case EnemyType::TANK: // Heavy armored vehicle, slow
            m_hpMax  = 350;
            m_speed  = 55.f;
            m_reward = 30;
            m_damage = 30;
            break;
        case EnemyType::PLANE: // Aircraft, fast but fragile compared to tank
            m_hpMax  = 600;
            m_speed  = 45.f;
            m_reward = 80;
            m_damage = 50;
            break;
    }
    m_hpCurrent = m_hpMax;
    // Start position at first waypoint
    if (!m_path.empty())
        m_position = m_path[0];
}

// Load enemy sprite from disk and scale it to 48x48
bool Enemy::loadTexture(const std::string& path) {
    if (!m_texture.loadFromFile(path)) {
        std::cerr << "Failed to load enemy texture: " << path << "\n";
        return false;
    }
    m_sprite.setTexture(m_texture);
    auto sz = m_texture.getSize();
    m_sprite.setScale(48.f / sz.x, 48.f / sz.y);
    m_sprite.setOrigin(sz.x / 2.f, sz.y / 2.f); // Center origin for clean positioning
    m_sprite.setPosition(m_position);
    return true;
}

// Move enemy toward the next waypoint each frame
void Enemy::update(float deltaTime) {
    // Wait until spawn delay expires before moving
    if (m_spawnDelay > 0.f) {
        m_spawnDelay -= deltaTime;
        return;
    }

    if (m_pathIndex >= (int)m_path.size()) return;

    sf::Vector2f target    = m_path[m_pathIndex];
    sf::Vector2f direction = target - m_position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 2px threshold avoids overshooting the waypoint
    if (distance < 2.f) {
        m_pathIndex++;
    } else {
        sf::Vector2f normalized = direction / distance;
        m_position += normalized * m_speed * deltaTime;
        m_sprite.setPosition(m_position);
        // Rotate sprite to face movement direction (atan2 gives angle of direction vector)
        float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI;
        m_sprite.setRotation(angle);
    }
}

void Enemy::render(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

// Returns true when the enemy has passed the last waypoint
bool Enemy::hasReachedBase() const { return m_pathIndex >= (int)m_path.size(); }

bool         Enemy::isDead()      const { return m_hpCurrent <= 0; }
int          Enemy::getReward()   const { return m_reward; }
EnemyType    Enemy::getType()     const { return m_type; }
sf::Vector2f Enemy::getPosition() const { return m_position; }

void Enemy::setSpawnDelay(float seconds) { m_spawnDelay = seconds; }
