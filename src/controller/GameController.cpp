#include "../../include/controller/GameController.hpp"
#include "../../include/controller/WaveController.hpp"

// These includes will be available once the Model/View classes are implemented
// #include "../../include/model/Game.hpp"
// #include "../../include/view/GameView.hpp"

#include <algorithm>
#include <iostream>

// ============================================================
//  Construction
// ============================================================

GameController::GameController(sf::RenderWindow&         window,
                               std::shared_ptr<Game>     game,
                               std::shared_ptr<GameView> gameView)
    : m_window(window)
    , m_game(std::move(game))
    , m_gameView(std::move(gameView))
{
    // Create the WaveController with a lambda that forwards wave events
    // to notifyObservers(). The lambda captures this by raw pointer:
    // WaveController is guaranteed to outlive GameController.
    m_waveController = std::make_unique<WaveController>(
        m_game,
        10,     // Total number of waves
        10.0f,  // Preparation timer: 10 seconds
        [this](GameEvent event, int value) {
            notifyObservers(event, value);
        }
    );

    // Subscribe the view as the first observer
    // (GameView must implement IObserver)
    subscribe(m_gameView);
}

// ============================================================
//  Observer Pattern — ISubject
// ============================================================

void GameController::subscribe(std::shared_ptr<IObserver> observer)
{
    // Store a weak_ptr to avoid extending the lifetime of observers:
    // the controller does not own the view.
    m_observers.emplace_back(observer);
}

void GameController::unsubscribe(std::shared_ptr<IObserver> observer)
{
    // Remove the matching weak_ptr (expired or same pointer)
    m_observers.erase(
        std::remove_if(m_observers.begin(), m_observers.end(),
            [&observer](const std::weak_ptr<IObserver>& wp) {
                auto sp = wp.lock();
                return !sp || sp == observer;
            }),
        m_observers.end()
    );
}

void GameController::notifyObservers(GameEvent event, int value)
{
    // Iterate while cleaning up expired weak_ptr entries
    auto it = m_observers.begin();
    while (it != m_observers.end()) {
        if (auto sp = it->lock()) {
            sp->onNotify(event, value);
            ++it;
        } else {
            // Observer was destroyed — remove the dead entry
            it = m_observers.erase(it);
        }
    }
}

// ============================================================
//  Main loop
// ============================================================

void GameController::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        m_window.close();
        m_isRunning = false;
        return;
    }

    // -- Keyboard --
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Escape:
                onQuit();
                break;

            case sf::Keyboard::P:
                onTogglePause();
                break;

            case sf::Keyboard::Space:
                // Manually start the next wave
                onStartWave();
                break;

            // Quick tower selection via number keys
            case sf::Keyboard::Num1:
                onTowerTypeSelected(TowerType::ARCHER);
                break;
            case sf::Keyboard::Num2:
                onTowerTypeSelected(TowerType::MAGE);
                break;
            case sf::Keyboard::Num3:
                onTowerTypeSelected(TowerType::CANNON);
                break;

            default:
                break;
        }
    }

    // -- Mouse (left click) --
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

        // Priority 1: upgrade an existing tower
        onTowerUpgrade(mousePos);

        // Priority 2: place a new tower
        onTowerPlacement(mousePos, m_selectedTowerType);
    }
}

void GameController::update(float deltaTime)
{
    if (m_isPaused || !m_isRunning) return;

    // 1. Update the wave system (spawning, inter-wave timer)
    m_waveController->update(deltaTime);

    // 2. Update the game model (enemy movement, tower attacks)
    // m_game->update(deltaTime);

    // 3. Check game-state conditions
    checkEnemyDeaths();
    checkBaseReached();
    checkGameOver();
}

void GameController::render()
{
    m_window.clear();

    // Delegate all drawing to the view
    // m_gameView->draw(m_window);

    m_window.display();
}

bool GameController::isRunning() const
{
    return m_isRunning && m_window.isOpen();
}

// ============================================================
//  Player actions
// ============================================================

void GameController::onTowerPlacement(sf::Vector2i mousePos, TowerType type)
{
    sf::Vector2i gridPos = pixelToGrid(mousePos);

    // Validate via the model (cell free + enough gold)
    // if (!m_game->getMap().isPlaceable(gridPos)) return;

    int cost = TowerFactory::getCost(type);
    // if (m_game->getGold() < cost) return;

    // Create via Factory (Factory Pattern) — unique_ptr ownership transferred to model
    auto tower = TowerFactory::create(type, gridPos.x, gridPos.y);
    if (!tower) return;

    // m_game->addTower(std::move(tower));
    // m_game->spendGold(cost);

    // Notify all observers (view, sound, score...)
    notifyObservers(GameEvent::TOWER_PLACED, cost);
    notifyObservers(GameEvent::GOLD_CHANGED, 0 /* m_game->getGold() */);
}

void GameController::onTowerUpgrade(sf::Vector2i mousePos)
{
    sf::Vector2i gridPos = pixelToGrid(mousePos);

    // Retrieve the tower at these coordinates (nullptr if cell is empty)
    // Tower* tower = m_game->getTowerAt(gridPos);
    // if (!tower) return;

    // Upgrade cost (may depend on the current tower level)
    // int upgradeCost = tower->getUpgradeCost();
    // if (m_game->getGold() < upgradeCost) return;

    // tower->upgrade();
    // m_game->spendGold(upgradeCost);

    notifyObservers(GameEvent::TOWER_UPGRADED, 0 /* upgradeCost */);
    notifyObservers(GameEvent::GOLD_CHANGED,   0 /* m_game->getGold() */);
}

void GameController::onTowerTypeSelected(TowerType type)
{
    m_selectedTowerType = type;
    // The view can highlight the corresponding button
    // m_gameView->setSelectedTower(type);
}

void GameController::onStartWave()
{
    m_waveController->forceStartNextWave();
}

void GameController::onTogglePause()
{
    m_isPaused = !m_isPaused;
    // m_gameView->showPauseOverlay(m_isPaused);
}

void GameController::onQuit()
{
    m_isRunning = false;
    // m_gameView->showMainMenu();
}

// ============================================================
//  Private methods
// ============================================================

sf::Vector2i GameController::pixelToGrid(sf::Vector2i mousePos) const
{
    // Cell size in pixels (must match Map / GameView settings)
    constexpr int CELL_SIZE = 64;
    return {mousePos.x / CELL_SIZE, mousePos.y / CELL_SIZE};
}

void GameController::checkEnemyDeaths()
{
    // Iterate over all enemies in the model
    // for (auto& enemyPtr : m_game->getEnemies()) {
    //     if (enemyPtr->isDead()) {
    //         int reward = enemyPtr->getReward();
    //         m_game->addGold(reward);
    //         m_game->addScore(reward * 10);
    //
    //         // Notify: gold earned and score updated
    //         notifyObservers(GameEvent::ENEMY_DEAD,    reward);
    //         notifyObservers(GameEvent::GOLD_CHANGED,  m_game->getGold());
    //         notifyObservers(GameEvent::SCORE_CHANGED, m_game->getScore());
    //     }
    // }
    // m_game->removeDeadEnemies();
}

void GameController::checkBaseReached()
{
    // for (auto& enemyPtr : m_game->getEnemies()) {
    //     if (enemyPtr->hasReachedBase()) {
    //         m_game->removeBaseHp(1);
    //         notifyObservers(GameEvent::ENEMY_REACHED_BASE, m_game->getBaseHp());
    //     }
    // }
}

void GameController::checkGameOver()
{
    // Loss condition: base has no HP left
    // if (m_game->getBaseHp() <= 0) {
    //     m_isRunning = false;
    //     notifyObservers(GameEvent::GAME_OVER, m_game->getScore());
    //     return;
    // }

    // Win condition: all waves cleared and no enemies remaining on the map
    // if (m_waveController->allWavesCleared() && m_game->getEnemies().empty()) {
    //     m_isRunning = false;
    //     notifyObservers(GameEvent::GAME_WIN, m_game->getScore());
    // }
}