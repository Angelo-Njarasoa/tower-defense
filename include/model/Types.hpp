#pragma once

// Centralized type enums shared by Tower, Enemy, and Factory
enum class TowerType {
    GATLING,  // Rapid fire, low damage per shot
    CANNON,   // Medium damage, medium range
    ROCKET    // Heavy damage, slow fire rate
};

enum class EnemyType {
    JEEP,   // Fast scout vehicle, low HP, low reward
    TANK,   // Heavy armored vehicle, slow, medium reward
    PLANE   // Aircraft, high HP, high reward
};
