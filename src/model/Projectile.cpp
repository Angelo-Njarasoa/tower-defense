#include "../../include/model/Projectile.hpp"
#include "../../include/model/Enemy.hpp"
#include <cmath>

static constexpr float PI = 3.14159265f;

static const char* texturePath(TowerType type) {
    switch (type) {
        case TowerType::GATLING: return "assets/proj_bullet.png";
        case TowerType::CANNON:  return "assets/proj_cannon.png";
        case TowerType::ROCKET:  return "assets/proj_rocket.png";
    }
    return "";
}

static float spriteSize(TowerType type) {
    switch (type) {
        case TowerType::GATLING: return 14.f;
        case TowerType::CANNON:  return 18.f;
        case TowerType::ROCKET:  return 28.f;
    }
    return 16.f;
}

Projectile::Projectile(TowerType type, sf::Vector2f start,
                       std::shared_ptr<Enemy> target, int damage)
    : m_type(type), m_pos(start), m_target(target), m_damage(damage)
{
    switch (type) {
        case TowerType::GATLING: m_speed = 450.f; break;
        case TowerType::CANNON:  m_speed = 280.f; break;
        case TowerType::ROCKET:  m_speed = 200.f; break;
    }

    if (m_texture.loadFromFile(texturePath(type))) {
        m_hasSprite = true;
        float sz = spriteSize(type);
        auto  tsz = m_texture.getSize();
        m_sprite.setTexture(m_texture);
        m_sprite.setOrigin(tsz.x / 2.f, tsz.y / 2.f);
        m_sprite.setScale(sz / tsz.x, sz / tsz.y);
        m_sprite.setPosition(m_pos);
    }
}

bool Projectile::update(float dt) {
    auto tgt = m_target.lock();
    if (!tgt || tgt->isDead() || tgt->hasReachedBase()) { m_dead = true; return false; }

    sf::Vector2f diff = tgt->getPosition() - m_pos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist < 10.f) {
        tgt->takeDamage(m_damage);
        m_dead = true;
        return false;
    }

    sf::Vector2f dir = diff / dist;
    m_pos  += dir * m_speed * dt;
    m_angle = std::atan2(diff.y, diff.x) * 180.f / PI;

    if (m_hasSprite) {
        m_sprite.setPosition(m_pos);
        // Only rotate the rocket sprite; bullets and cannonballs are circular
        if (m_type == TowerType::ROCKET)
            m_sprite.setRotation(m_angle + 90.f);
    }
    return true;
}

void Projectile::render(sf::RenderWindow& window) const {
    if (m_hasSprite) {
        window.draw(m_sprite);
        return;
    }

    // Fallback shapes if texture failed to load
    switch (m_type) {
        case TowerType::GATLING: {
            sf::CircleShape c(4.f);
            c.setOrigin(4.f, 4.f);
            c.setPosition(m_pos);
            c.setFillColor(sf::Color(255, 255, 80));
            window.draw(c);
            break;
        }
        case TowerType::CANNON: {
            sf::CircleShape c(6.f);
            c.setOrigin(6.f, 6.f);
            c.setPosition(m_pos);
            c.setFillColor(sf::Color(55, 55, 55));
            window.draw(c);
            break;
        }
        case TowerType::ROCKET: {
            sf::RectangleShape r(sf::Vector2f(5.f, 14.f));
            r.setOrigin(2.5f, 7.f);
            r.setPosition(m_pos);
            r.setRotation(m_angle + 90.f);
            r.setFillColor(sf::Color(255, 70, 30));
            window.draw(r);
            break;
        }
    }
}
