#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

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

    // Tracks which grid tiles already have a tower (prevents double placement)
    std::set<std::pair<int,int>> occupiedTiles = { {2,1}, {5,5}, {9,3} };

    // Player base HP — loses 1 life per enemy that reaches the base
    int playerHP    = 10;
    int playerMaxHP = 10;

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

            // Left click: place a GATLING tower on a free grass tile
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                int col = event.mouseButton.x / TILE_SIZE;
                int row = event.mouseButton.y / TILE_SIZE;
                std::pair<int,int> tile = {col, row};
                if (map.isGrass(col, row) && occupiedTiles.find(tile) == occupiedTiles.end()) {
                    auto t = std::make_shared<Tower>(TowerType::GATLING, sf::Vector2i(col, row));
                    t->loadTexture("assets/tower_gatling.png");
                    towers.push_back(t);
                    occupiedTiles.insert(tile);
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // Update enemies (movement along path)
        for (auto& enemy : enemies)
            enemy->update(dt);

        // Subtract 1 life for each enemy that reached the base
        for (auto& e : enemies)
            if (e->hasReachedBase()) playerHP--;
        if (playerHP <= 0) window.close();

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

        // Draw HP bar at top-left (background then fill)
        sf::RectangleShape hpBg(sf::Vector2f(204.f, 18.f));
        hpBg.setFillColor(sf::Color(60, 0, 0));
        hpBg.setPosition(10.f, 10.f);
        window.draw(hpBg);

        float ratio = static_cast<float>(playerHP) / playerMaxHP;
        sf::RectangleShape hpFill(sf::Vector2f(200.f * ratio, 14.f));
        hpFill.setFillColor(sf::Color(220, 30, 30));
        hpFill.setPosition(12.f, 12.f);
        window.draw(hpFill);

        window.display();
    }

    return 0;
}
