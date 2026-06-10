#include "../../include/view/GameView.hpp"
#include "../../include/model/Game.hpp"
#include "../../include/controller/WaveController.hpp"
#include "../../include/patterns/Factory.hpp"
#include <algorithm>

static constexpr int HUD_HEIGHT  = 44;
static constexpr int MAP_HEIGHT  = 576; // 9 rows * 64px

GameView::GameView(sf::RenderWindow& window, std::shared_ptr<Game> game)
    : m_window(window), m_game(std::move(game))
{
    // Try several font locations; fall back to shape-only HUD if none found
    if      (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))                               m_hasFont = true;
    else if (m_font.loadFromFile("C:/Windows/Fonts/segoeui.ttf"))                             m_hasFont = true;
    else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))          m_hasFont = true;
    else if (m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"))            m_hasFont = true;
}

void GameView::render(const WaveController* wave) {
    m_window.clear(sf::Color(20, 20, 20));

    m_game->map.draw(m_window);

    for (auto& t : m_game->towers)
        t->render(m_window);

    for (auto& e : m_game->enemies)
        e->render(m_window);

    drawHud(wave);

    if (m_gameOver) drawOverlay("GAME OVER  —  ESC to quit", sf::Color(180, 0, 0, 210));
    if (m_gameWin)  drawOverlay("YOU WIN!  —  ESC to quit",   sf::Color(0, 150, 0,  210));

    m_window.display();
}

void GameView::drawHud(const WaveController* wave) {
    const float W   = static_cast<float>(m_window.getSize().x);
    const float TOP = static_cast<float>(MAP_HEIGHT);

    // Background strip
    sf::RectangleShape bg(sf::Vector2f(W, HUD_HEIGHT));
    bg.setFillColor(sf::Color(25, 25, 25, 230));
    bg.setPosition(0.f, TOP);
    m_window.draw(bg);

    // Lives: 10 small hearts (red = alive, dark = lost)
    for (int i = 0; i < 10; ++i) {
        sf::RectangleShape h(sf::Vector2f(13.f, 13.f));
        h.setPosition(10.f + i * 17.f, TOP + 15.f);
        h.setFillColor(i < m_game->lives ? sf::Color(220, 40, 40) : sf::Color(70, 20, 20));
        m_window.draw(h);
    }

    // Gold bar (yellow fill, 0–300 scale)
    float gRatio = std::min(1.f, m_game->gold / 300.f);
    sf::RectangleShape gBg(sf::Vector2f(100.f, 12.f));
    gBg.setFillColor(sf::Color(55, 45, 0));
    gBg.setPosition(210.f, TOP + 16.f);
    m_window.draw(gBg);
    sf::RectangleShape gFill(sf::Vector2f(100.f * gRatio, 12.f));
    gFill.setFillColor(sf::Color(255, 200, 0));
    gFill.setPosition(210.f, TOP + 16.f);
    m_window.draw(gFill);

    // Tower palette (3 buttons — highlight not needed for now)
    const sf::Color btnColors[] = { sf::Color(50,100,50), sf::Color(50,50,100), sf::Color(100,50,50) };
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape btn(sf::Vector2f(68.f, 28.f));
        btn.setPosition(340.f + i * 78.f, TOP + 8.f);
        btn.setFillColor(btnColors[i]);
        btn.setOutlineColor(sf::Color(150, 150, 150));
        btn.setOutlineThickness(1.f);
        m_window.draw(btn);
    }

    // Wave progress dots
    if (wave) {
        int cur   = wave->getCurrentWaveNumber();
        int total = wave->getTotalWaves();
        for (int i = 0; i < total && i < 10; ++i) {
            sf::RectangleShape dot(sf::Vector2f(9.f, 16.f));
            dot.setPosition(630.f + i * 12.f, TOP + 14.f);
            sf::Color col = (i < cur - 1) ? sf::Color(60, 210, 60)
                          : (i == cur - 1) ? sf::Color(255, 220, 0)
                          :                  sf::Color(70, 70, 70);
            dot.setFillColor(col);
            m_window.draw(dot);
        }
    }

    // Text overlay (only when font is available)
    if (!m_hasFont) return;

    auto label = [&](const std::string& s, float x, float y, unsigned int sz = 12) {
        sf::Text t(s, m_font, sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(x, y);
        m_window.draw(t);
    };

    label("HP",                             10.f,  TOP + 2.f);
    label("Gold: " + std::to_string(m_game->gold), 210.f, TOP + 2.f);
    label("Score: " + std::to_string(m_game->score), 210.f, TOP + 30.f, 10);

    // Tower buttons labels
    const char* tNames[] = {"1-GAT $50", "2-CAN $80", "3-ROC $120"};
    for (int i = 0; i < 3; ++i)
        label(tNames[i], 343.f + i * 78.f, TOP + 15.f, 10);

    // Wave info
    if (wave) {
        std::string ws = "Wave " + std::to_string(wave->getCurrentWaveNumber())
                       + "/" + std::to_string(wave->getTotalWaves());
        label(ws, 630.f, TOP + 2.f);
        if (!wave->isWaveInProgress() && !wave->allWavesCleared()) {
            int secs = static_cast<int>(wave->getPrepTimeRemaining()) + 1;
            label("SPACE=" + std::to_string(secs) + "s", 630.f, TOP + 28.f, 10);
        }
    }
}

void GameView::drawOverlay(const std::string& msg, sf::Color color) {
    sf::RectangleShape box(sf::Vector2f(480.f, 80.f));
    box.setFillColor(color);
    box.setPosition(144.f, 248.f);
    m_window.draw(box);

    if (m_hasFont) {
        sf::Text t(msg, m_font, 26);
        t.setFillColor(sf::Color::White);
        // Center text in box
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin(r.width / 2.f, r.height / 2.f);
        t.setPosition(384.f, 288.f);
        m_window.draw(t);
    }
}

void GameView::onNotify(GameEvent event, int /*value*/) {
    if (event == GameEvent::GAME_OVER) m_gameOver = true;
    if (event == GameEvent::GAME_WIN)  m_gameWin  = true;
}
