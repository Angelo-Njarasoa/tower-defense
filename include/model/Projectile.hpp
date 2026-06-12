#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Types.hpp"

class Enemy;

class Projectile {
public:
    Projectile(TowerType type, sf::Vector2f start,
               std::shared_ptr<Enemy> target, int damage);

    bool update(float dt);  // returns false when expired
    void render(sf::RenderWindow& window) const;
    bool isDead() const { return m_dead; }

private:
    TowerType            m_type;
    sf::Vector2f         m_pos;
    std::weak_ptr<Enemy> m_target;
    int                  m_damage;
    float                m_speed;
    float                m_angle = 0.f;
    bool                 m_dead  = false;
    sf::Texture          m_texture;
    sf::Sprite           m_sprite;
    bool                 m_hasSprite = false;
};
