#include "../../include/controller/GameController.hpp"
#include "../../include/controller/WaveController.hpp"

#include <algorithm>
#include <iostream>

//  Construction
GameController::GameController(sf::RenderWindow&         window,
                               std::shared_ptr<Game>     game,
                               std::shared_ptr<GameView> gameView)
    : m_window(window)
    , m_game(std::move(game))
    , m_gameView(std::move(gameView))
{
    // Create the WaveController with a lambda that forwards wave events
    m_waveController = std::make_unique<WaveController>(
        m_game,
        10,     // Total number of waves
        10.0f,  // Preparation timer: 10 seconds
        [this](GameEvent event, int value) {
            notifyObservers(event, value);
        }
    );

    subscribe(m_gameView);
}

//  Observer Pattern
void GameController::subscribe(std::shared_ptr<IObserver> observer)
{
    // Store a weak_ptr to avoid extending the lifetime of observers: the controller does not own the view.
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
            it = m_observers.erase(it);
        }
    }
}

//  Main loop
void GameController::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        m_window.close();
        m_isRunning = false;
        return;
    }

    // Keyboard
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

    //  Mouse (left click)
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

        onTowerUpgrade(mousePos);

        onTowerPlacement(mousePos, m_selectedTowerType);
    }
}

void GameController::update(float deltaTime)
{
    if (m_isPaused || !m_isRunning) return;

    m_waveController->update(deltaTime);

    checkEnemyDeaths();
    checkBaseReached();
    checkGameOver();
}

void GameController::render()
{
    m_window.clear();
    m_window.display();
}

bool GameController::isRunning() const
{
    return m_isRunning && m_window.isOpen();
}

//  Player actions
void GameController::onTowerPlacement(sf::Vector2i mousePos, TowerType type)
{
    sf::Vector2i gridPos = pixelToGrid(mousePos);

    int cost = TowerFactory::getCost(type);

    // Create via Factory (Factory Pattern) — unique_ptr ownership transferred to model
    auto tower = TowerFactory::create(type, gridPos.x, gridPos.y);
    if (!tower) return;

    // Notify all observers (view, sound, score...)
    notifyObservers(GameEvent::TOWER_PLACED, cost);
    notifyObservers(GameEvent::GOLD_CHANGED, 0);
}

void GameController::onTowerUpgrade(sf::Vector2i mousePos)
{
    sf::Vector2i gridPos = pixelToGrid(mousePos);

    notifyObservers(GameEvent::TOWER_UPGRADED, 0);
    notifyObservers(GameEvent::GOLD_CHANGED,   0 );
}

void GameController::onTowerTypeSelected(TowerType type)
{
    m_selectedTowerType = type;
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

//  Private methods
sf::Vector2i GameController::pixelToGrid(sf::Vector2i mousePos) const
{
    // Cell size in pixels (must match Map / GameView settings)
    constexpr int CELL_SIZE = 64;
    return {mousePos.x / CELL_SIZE, mousePos.y / CELL_SIZE};
}

void GameController::checkEnemyDeaths()
{
}

void GameController::checkBaseReached()
{
}

void GameController::checkGameOver()
{
}