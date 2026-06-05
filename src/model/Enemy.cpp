#include "../../include/model/Enemy.hpp"

// Sets stats based on enemy type; hpCurrent starts at full health
Enemy::Enemy(EnemyType type, std::vector<sf::Vector2f> path) {
    m_type      = type;
    m_path      = path;
    m_pathIndex = 0;

    switch (type) {
        case EnemyType::GOBLIN: // fast but fragile
            m_hpMax  = 100;
            m_speed  = 100.0f;
            m_reward = 10;
            m_damage = 10;
            break;
        case EnemyType::TROLL: // slow and tanky
            m_hpMax  = 250;
            m_speed  = 60.0f;
            m_reward = 25;
            m_damage = 25;
            break;
        case EnemyType::BOSS: // very slow, very dangerous
            m_hpMax  = 800;
            m_speed  = 40.0f;
            m_reward = 100;
            m_damage = 50;
            break;
    }
    m_hpCurrent = m_hpMax;
}

// Moves enemy toward the next waypoint; advances index when close enough
void Enemy::update(float deltaTime) {
    if (m_pathIndex >= (int)m_path.size()) return;

    sf::Vector2f target    = m_path[m_pathIndex];
    sf::Vector2f direction = target - m_position;
    float distance         = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 2px threshold avoids overshooting the waypoint
    if (distance < 2.0f) {
        m_pathIndex++;
    } else {
        sf::Vector2f normalized = direction / distance;
        m_position += normalized * m_speed * deltaTime;
        m_sprite.setPosition(m_position);
    }
}

void Enemy::render(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

// True when the enemy has passed the last waypoint
bool Enemy::hasReachedBase() {
    return m_pathIndex >= (int)m_path.size();
}

bool Enemy::isDead() {
    return m_hpCurrent <= 0;
}

int Enemy::getReward() {
    return m_reward;
}

EnemyType Enemy::getType() {
    return m_type;
}
