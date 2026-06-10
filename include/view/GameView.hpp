#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../patterns/Observer.hpp"

class Game;
class WaveController;

class GameView : public IObserver {
public:
    GameView(sf::RenderWindow& window, std::shared_ptr<Game> game);
    ~GameView() override = default;

    void render(const WaveController* wave);
    void onNotify(GameEvent event, int value) override;

private:
    sf::RenderWindow&     m_window;
    std::shared_ptr<Game> m_game;
    sf::Font              m_font;
    bool                  m_hasFont  = false;
    bool                  m_gameOver = false;
    bool                  m_gameWin  = false;

    void drawHud(const WaveController* wave);
    void drawOverlay(const std::string& msg, sf::Color color);
};
