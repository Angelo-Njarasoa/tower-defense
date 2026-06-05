#pragma once
#include <SFML/Graphics.hpp>

class Entity {
protected:
    int          m_hpMax;
    int          m_hpCurrent;
    sf::Sprite   m_sprite;
    sf::Vector2f m_position;

public:
    bool         takeDamage(int damage);
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow&) const = 0;
    virtual ~Entity() = default;
};