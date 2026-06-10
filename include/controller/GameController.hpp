#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../patterns/Observer.hpp"
#include "../patterns/Factory.hpp"
#include "WaveController.hpp"

class Game;
class GameView;

class GameController : public ISubject {
public:
    GameController(sf::RenderWindow&         window,
                   std::shared_ptr<Game>     game,
                   std::shared_ptr<GameView> gameView);
    ~GameController() override = default;
    GameController(const GameController&)            = delete;
    GameController& operator=(const GameController&) = delete;

    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void render();
    bool isRunning() const;

    void subscribe(std::shared_ptr<IObserver> observer) override;
    void unsubscribe(std::shared_ptr<IObserver> observer) override;
    void notifyObservers(GameEvent event, int value = 0) override;

    void onTowerPlacement(sf::Vector2i mousePos, TowerType type);
    void onTowerUpgrade(sf::Vector2i mousePos);
    void onTowerTypeSelected(TowerType type);
    void onStartWave();
    void onTogglePause();
    void onQuit();

private:
    sf::RenderWindow&               m_window;
    std::shared_ptr<Game>           m_game;
    std::shared_ptr<GameView>       m_gameView;
    std::unique_ptr<WaveController> m_waveController;

    TowerType m_selectedTowerType = TowerType::GATLING;
    bool      m_isPaused          = false;
    bool      m_isRunning         = true;
    bool      m_gameOver          = false;
    bool      m_gameWin           = false;

    sf::Vector2i pixelToGrid(sf::Vector2i mousePos) const;
    void checkEnemyDeaths();
    void checkBaseReached();
    void checkGameOver();
};
