
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
        case TowerType::GATLING: // Rapid fire, low damage per shot
            m_damage      = 10;
            m_range       = 160.f;
            m_fireRate    = 3.0f;   // 3 shots per second
            m_cost        = 50;
            m_upgradeCost = 30;
            break;
        case TowerType::CANNON: // Medium damage, medium range
            m_damage      = 40;
            m_range       = 130.f;
            m_fireRate    = 0.9f;
            m_cost        = 80;
            m_upgradeCost = 50;
            break;
        case TowerType::ROCKET: // Heavy damage, slow fire rate
            m_damage      = 80;
            m_range       = 110.f;
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

// Load the base platform (always tower_base.png) and the type-specific gun sprite
bool Tower::loadTexture(const std::string& gunPath) {
    // Base: fills the whole 64x64 tile, never rotates
    if (m_texBase.loadFromFile("assets/tower_base.png")) {
        m_spriteBase.setTexture(m_texBase);
        auto bsz = m_texBase.getSize();
        m_spriteBase.setScale(64.f / bsz.x, 64.f / bsz.y);
        m_spriteBase.setPosition(m_position);
    }

    // Gun: 48x48, centered on the tile, rotates toward enemies
    if (!m_texGun.loadFromFile(gunPath)) {
        std::cerr << "Failed to load tower texture: " << gunPath << "\n";
        return false;
    }
    m_sprite.setTexture(m_texGun);
    auto sz = m_texGun.getSize();
    m_sprite.setScale(48.f / sz.x, 48.f / sz.y);
    m_sprite.setOrigin(sz.x / 2.f, sz.y / 2.f);
    m_sprite.setPosition(m_position.x + 32.f, m_position.y + 32.f);
    // Distinct tint per type so towers are visually unambiguous
    switch (m_type) {
        case TowerType::GATLING: m_sprite.setColor(sf::Color(160, 255, 160)); break; // green
        case TowerType::CANNON:  m_sprite.setColor(sf::Color(160, 200, 255)); break; // blue
        case TowerType::ROCKET:  m_sprite.setColor(sf::Color(255, 160, 160)); break; // red
    }
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
    window.draw(m_spriteBase); // base platform first
    window.draw(m_sprite);     // gun on top
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
            // +90 offset because the sprite faces up by default (0deg = up, not right)
            float angle = std::atan2(diff.y, diff.x) * 180.f / M_PI + 90.f;
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
