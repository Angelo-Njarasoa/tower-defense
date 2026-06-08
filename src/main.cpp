#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

#include "../include/model/Map.hpp"
#include "../include/model/Tower.hpp"
#include "../include/model/Enemy.hpp"

int main()
{
    // Window size matches the tilemap: 12 cols x 9 rows x 64px = 768x576
    sf::RenderWindow window(sf::VideoMode(768, 576), "Tower Defense - Visual Test");
    window.setFramerateLimit(60);

    // --- Map ---
    Map map;
    if (!map.load("assets/grass.png", "assets/path.png", "assets/tower_base.png"))
        return -1;

    // Retrieve waypoints computed by the map (enemies follow this path)
    const std::vector<sf::Vector2f>& waypoints = map.getWaypoints();

    // --- Towers ---
    auto gatling = std::make_shared<Tower>(TowerType::GATLING, sf::Vector2i(2, 1));
    gatling->loadTexture("assets/tower_gatling.png");

    auto cannon = std::make_shared<Tower>(TowerType::CANNON, sf::Vector2i(5, 5));
    cannon->loadTexture("assets/tower_cannon.png");

    auto rocket = std::make_shared<Tower>(TowerType::ROCKET, sf::Vector2i(9, 3));
    rocket->loadTexture("assets/tower_rocket.png");

    std::vector<std::shared_ptr<Tower>> towers = { gatling, cannon, rocket };

    // --- Enemies ---
    auto jeep = std::make_shared<Enemy>(EnemyType::JEEP, waypoints);
    jeep->loadTexture("assets/enemy_jeep.png");

    auto tank = std::make_shared<Enemy>(EnemyType::TANK, waypoints);
    tank->loadTexture("assets/enemy_tank.png");
    tank->setSpawnDelay(3.0f);

    auto plane = std::make_shared<Enemy>(EnemyType::PLANE, waypoints);
    plane->loadTexture("assets/enemy_plane.png");
    plane->setSpawnDelay(6.0f);

    std::vector<std::shared_ptr<Enemy>> enemies = { jeep, tank, plane };

    sf::Clock clock;

    // --- Main loop ---
    while (window.isOpen())
    {
        // Handle window and keyboard events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // Update enemies (movement along path)
        for (auto& enemy : enemies)
            enemy->update(dt);

        // Remove enemies that reached the base or died
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::shared_ptr<Enemy>& e){ return e->hasReachedBase() || e->isDead(); }),
            enemies.end()
        );

        // Update towers (cooldown) and trigger attacks
        for (auto& tower : towers)
        {
            tower->update(dt);
            tower->attack(enemies);
        }

        // --- Render ---
        window.clear();
        map.draw(window);

        for (auto& tower : towers)
            tower->render(window);

        for (auto& enemy : enemies)
            enemy->render(window);

        window.display();
    }

    return 0;
}
