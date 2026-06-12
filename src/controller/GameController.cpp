#include "../../include/controller/GameController.hpp"
#include "../../include/controller/WaveController.hpp"
#include "../../include/view/GameView.hpp"
#include "../../include/model/Game.hpp"
#include <algorithm>
#include <iostream>

GameController::GameController(sf::RenderWindow&         window,
                               std::shared_ptr<Game>     game,
                               std::shared_ptr<GameView> gameView)
    : m_window(window)
    , m_game(std::move(game))
    , m_gameView(std::move(gameView))
{
    m_waveController = std::make_unique<WaveController>(
        m_game,
        10,     // 10 waves total
        10.0f,  // 10-second prep timer between waves
        [this](GameEvent event, int value) { notifyObservers(event, value); }
    );

    // Register GameView as an observer so it reacts to GAME_OVER / GAME_WIN
    subscribe(m_gameView);
}

// ISubject
void GameController::subscribe(std::shared_ptr<IObserver> observer) {
    m_observers.emplace_back(observer);
}

void GameController::unsubscribe(std::shared_ptr<IObserver> observer) {
    m_observers.erase(
        std::remove_if(m_observers.begin(), m_observers.end(),
            [&observer](const std::weak_ptr<IObserver>& wp) {
                auto sp = wp.lock();
                return !sp || sp == observer;
            }),
        m_observers.end()
    );
}

void GameController::notifyObservers(GameEvent event, int value) {
    auto it = m_observers.begin();
    while (it != m_observers.end()) {
        if (auto sp = it->lock()) {
            sp->onNotify(event, value);
            ++it;
        } else {
            it = m_observers.erase(it);
        }
    }
}

// Event handling
void GameController::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        m_window.close();
        return;
    }

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Escape: m_window.close(); break;
            case sf::Keyboard::P:      onTogglePause();   break;
            case sf::Keyboard::Space:  onStartWave();     break;
            case sf::Keyboard::Num1:   onTowerTypeSelected(TowerType::GATLING); break;
            case sf::Keyboard::Num2:   onTowerTypeSelected(TowerType::CANNON);  break;
            case sf::Keyboard::Num3:   onTowerTypeSelected(TowerType::ROCKET);  break;
            default: break;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f world = m_window.mapPixelToCoords(mousePos);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // Click on HUD tower buttons (y >= 576) selects tower type
            if (world.y >= 576.f) {
                for (int i = 0; i < 3; ++i) {
                    float bx = 340.f + i * 78.f;
                    if (world.x >= bx && world.x <= bx + 68.f)
                        onTowerTypeSelected(static_cast<TowerType>(i));
                }
            } else {
                onTowerPlacement(mousePos, m_selectedTowerType);
            }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            onTowerUpgrade(mousePos);
        }
    }
}

void GameController::update(float deltaTime) {
    if (m_isPaused || !m_isRunning) return;

    m_waveController->update(deltaTime);

    for (auto& e : m_game->enemies)
        e->update(deltaTime);

    for (auto& t : m_game->towers) {
        t->update(deltaTime);
        t->attack(m_game->enemies);
    }

    checkEnemyDeaths();
    checkBaseReached();

    // Remove enemies that are done (dead or reached base)
    m_game->enemies.erase(
        std::remove_if(m_game->enemies.begin(), m_game->enemies.end(),
            [](const std::shared_ptr<Enemy>& e) {
                return e->isDead() || e->hasReachedBase();
            }),
        m_game->enemies.end()
    );

    checkGameOver();

    // Win condition: all waves spawned and no enemies left
    if (!m_gameWin && m_waveController->allWavesCleared() && m_game->enemies.empty()) {
        m_gameWin  = true;
        m_isRunning = false;
        notifyObservers(GameEvent::GAME_WIN, 0);
    }
}

void GameController::render() {
    m_gameView->render(m_waveController.get(), m_selectedTowerType);
}

bool GameController::isRunning() const {
    return m_window.isOpen();
}

// Tower placement: validate cell, check gold, create and register tower
void GameController::onTowerPlacement(sf::Vector2i mousePos, TowerType type) {
    sf::Vector2i gridPos = pixelToGrid(mousePos);
    if (gridPos.x < 0 || gridPos.y < 0) return;

    if (!m_game->map.isGrass(gridPos.x, gridPos.y)) return;
    if (m_game->occupiedTiles.count({gridPos.x, gridPos.y})) return;

    int cost = TowerFactory::getCost(type);
    if (m_game->gold < cost) return;

    auto tower = TowerFactory::create(type, gridPos.x, gridPos.y);
    if (!tower) return;
    tower->loadTexture(TowerFactory::getTexturePath(type));

    m_game->gold -= cost;
    m_game->occupiedTiles.insert({gridPos.x, gridPos.y});
    m_game->towers.push_back(std::move(tower));

    notifyObservers(GameEvent::TOWER_PLACED, cost);
    notifyObservers(GameEvent::GOLD_CHANGED, m_game->gold);
}

// Tower upgrade: find tower at grid cell and upgrade if affordable
void GameController::onTowerUpgrade(sf::Vector2i mousePos) {
    sf::Vector2i gridPos = pixelToGrid(mousePos);
    if (gridPos.x < 0 || gridPos.y < 0) return;

    for (auto& tower : m_game->towers) {
        if (tower->getGridPosition() != gridPos) continue;
        if (!tower->canUpgrade()) return;

        int cost = tower->getUpgradeCost();
        if (m_game->gold < cost) return;

        m_game->gold -= cost;
        tower->upgrade();
        notifyObservers(GameEvent::TOWER_UPGRADED, cost);
        notifyObservers(GameEvent::GOLD_CHANGED, m_game->gold);
        return;
    }
}

void GameController::onTowerTypeSelected(TowerType type) {
    m_selectedTowerType = type;
}

void GameController::onStartWave() {
    m_waveController->forceStartNextWave();
}

void GameController::onTogglePause() {
    m_isPaused = !m_isPaused;
}

void GameController::onQuit() {
    m_window.close();
}

sf::Vector2i GameController::pixelToGrid(sf::Vector2i mousePos) const {
    // Convert screen pixels to logical game coords (handles any view/fullscreen scaling)
    sf::Vector2f world = m_window.mapPixelToCoords(mousePos);
    if (world.y >= 576.f) return {-1, -1};
    return {static_cast<int>(world.x / 64), static_cast<int>(world.y / 64)};
}

void GameController::checkEnemyDeaths() {
    for (auto& e : m_game->enemies) {
        if (!e->isDead()) continue;
        int reward = e->getReward();
        m_game->gold  += reward;
        m_game->score += reward * 10;
        notifyObservers(GameEvent::ENEMY_DEAD, reward);
        notifyObservers(GameEvent::GOLD_CHANGED, m_game->gold);
        notifyObservers(GameEvent::SCORE_CHANGED, m_game->score);
    }
}

void GameController::checkBaseReached() {
    for (auto& e : m_game->enemies) {
        if (!e->hasReachedBase()) continue;
        m_game->lives -= 1;
        if (m_game->lives < 0) m_game->lives = 0;
        notifyObservers(GameEvent::ENEMY_REACHED_BASE, 1);
    }
}

void GameController::checkGameOver() {
    if (!m_gameOver && m_game->lives <= 0) {
        m_gameOver  = true;
        m_isRunning = false;
        notifyObservers(GameEvent::GAME_OVER, 0);
    }
}
