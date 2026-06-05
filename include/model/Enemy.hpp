#pragma once
#include "../../include/model/entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
enum class EnemyType { GOBLIN, TROLL, BOSS };
class Enemy: public Entity
{
private:
    EnemyType m_type;
    float m_speed;
    int m_reward ;
    int m_damage;
    std::vector<sf::Vector2f> m_path;
    int m_pathIndex;
public:
    void update(float deltaTime)override;
    void render (sf::RenderWindow&)const;
    bool hasReachedBase();
    bool isDead();
    int getReward();
    EnemyType getType();
    Enemy(EnemyType type, std::vector<sf::Vector2f> path);
};
