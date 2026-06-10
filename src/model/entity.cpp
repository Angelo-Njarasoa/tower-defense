#include "../../include/model/entity.hpp"

bool Entity::takeDamage(int damage) {
    m_hpCurrent -= damage;
    return m_hpCurrent <= 0;
}
