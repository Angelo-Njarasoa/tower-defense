#include "../../include/model/Tower.hpp"
#include <cmath>
#include <iostream>

// Initialize tower stats based on its type and place it on the grid
Tower::Tower(TowerType type, sf::Vector2i gridPos)
    : m_type(type), m_gridPos(gridPos), m_upgradeLevel(0), m_fireCooldown(0.f),
      m_flashTimer(0.f), m_lastTargetPos(0.f, 0.f)
{
    // Each tower type has different damage, range, fire rate and cost
    switch (type) {
        case TowerType::ARCHER:
            m_damage      = 15;     // Low damage, high fire rate
            m_range       = 150.f;
            m_fireRate    = 1.5f;   // Shots per second
            m_cost        = 50;
            m_upgradeCost = 30;
            break;
        case TowerType::MAGE:
            m_damage      = 30;     // Medium damage and range
            m_range       = 120.f;
            m_fireRate    = 0.8f;
            m_cost        = 80;
            m_upgradeCost = 50;
            break;
        case TowerType::CANNON:
            m_damage      = 60;     // High damage, slow fire rate
            m_range       = 100.f;
            m_fireRate    = 0.4f;
            m_cost        = 120;
            m_upgradeCost = 80;
            break;
    }
    // Towers are indestructible in this version
    m_hpMax     = 999;
    m_hpCurrent = 999;
    // Convert grid coordinates to pixel position (64px per tile)
    m_position  = sf::Vector2f(gridPos.x * 64.f, gridPos.y * 64.f);
}

// Load the tower sprite from disk and scale it to fit the tile
bool Tower::loadTexture(const std::string& path) {
    if (!m_texture.loadFromFile(path)) {
        std::cerr << "Failed to load tower texture: " << path << "\n";
        return false;
    }
    m_sprite.setTexture(m_texture);
    auto sz = m_texture.getSize();
    // Scale to 48x48, center origin so rotation pivots around the middle of the sprite
    m_sprite.setScale(48.f / sz.x, 48.f / sz.y);
    m_sprite.setOrigin(sz.x / 2.f, sz.y / 2.f);
    m_sprite.setPosition(m_position.x + 32.f, m_position.y + 32.f);
    return true;
}

// Decrease fire cooldown and flash timer each frame
void Tower::update(float deltaTime) {
    if (m_fireCooldown > 0.f)
        m_fireCooldown -= deltaTime;
    if (m_flashTimer > 0.f)
        m_flashTimer -= deltaTime;
}

void Tower::render(sf::RenderWindow& window) const {
    window.draw(m_sprite);
    // Draw a yellow line toward the last target for 0.12s after each shot
    if (m_flashTimer > 0.f) {
        sf::Vector2f center = m_position + sf::Vector2f(32.f, 32.f);
        sf::Vertex line[] = {
            sf::Vertex(center,           sf::Color::Yellow),
            sf::Vertex(m_lastTargetPos,  sf::Color::Yellow)
        };
        window.draw(line, 2, sf::Lines);
    }
}

// Attack the first living enemy within range, then enter cooldown
void Tower::attack(std::vector<std::shared_ptr<Enemy>>& enemies) {
    if (m_fireCooldown > 0.f) return;

    sf::Vector2f center = m_position + sf::Vector2f(32.f, 32.f);
    for (auto& enemy : enemies) {
        if (!enemy || enemy->isDead() || enemy->hasReachedBase()) continue;
        sf::Vector2f diff = enemy->getPosition() - center;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist <= m_range) {
            // Rotate tower sprite to face the target
            float angle = std::atan2(diff.y, diff.x) * 180.f / M_PI;
            m_sprite.setRotation(angle);
            enemy->takeDamage(m_damage);
            m_lastTargetPos = enemy->getPosition(); // Store target position for visual
            m_flashTimer    = 0.12f;                // Activate attack line for 0.12s
            m_fireCooldown  = 1.f / m_fireRate;
            break; // One target per shot
        }
    }
}

// Each upgrade boosts damage (+30%), range (+10%) and fire rate (+20%)
void Tower::upgrade() {
    if (!canUpgrade()) return;
    m_upgradeLevel++;
    m_damage      = static_cast<int>(m_damage * 1.3f);
    m_range      *= 1.1f;
    m_fireRate   *= 1.2f;
    m_upgradeCost = static_cast<int>(m_upgradeCost * 1.5f);
}

bool         Tower::canUpgrade()      const { return m_upgradeLevel < 3; }
TowerType    Tower::getType()         const { return m_type; }
int          Tower::getCost()         const { return m_cost; }
int          Tower::getUpgradeCost()  const { return m_upgradeCost; }
float        Tower::getRange()        const { return m_range; }
sf::Vector2f Tower::getPosition()     const { return m_position; }
sf::Vector2i Tower::getGridPosition() const { return m_gridPos; }
