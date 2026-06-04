#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../patterns/Observer.hpp"
#include "../patterns/Factory.hpp"

// Forward declarations
class Game;
class GameView;
class WaveController;

// ============================================================
//  GameController
//
//  MVC role: bridges the Model (Game) and the View (GameView).
//  - Reads keyboard/mouse input
//  - Updates the Model
//  - Asks the View to redraw
//
//  Implements ISubject to broadcast game events
//  to all observers (GameView, SoundManager, ScoreManager...)
// ============================================================
class GameController : public ISubject {
public:
    // ----------------------------------------------------------
    //  Construction / Destruction
    // ----------------------------------------------------------

    /**
     * @brief Main constructor
     * @param window    Reference to the SFML window (externally owned)
     * @param game      Shared pointer to the game model
     * @param gameView  Shared pointer to the main view
     */
    GameController(sf::RenderWindow&         window,
                   std::shared_ptr<Game>     game,
                   std::shared_ptr<GameView> gameView);

    ~GameController() override = default;

    // No copy — one controller per game session
    GameController(const GameController&)            = delete;
    GameController& operator=(const GameController&) = delete;

    // ----------------------------------------------------------
    //  Main loop
    // ----------------------------------------------------------

    /**
     * @brief Processes an SFML event (keyboard, mouse, window close...)
     * @param event  Event received from the main loop
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Updates the game logic
     * @param deltaTime  Time elapsed since the last frame (seconds)
     */
    void update(float deltaTime);

    /**
     * @brief Asks the view to redraw everything
     */
    void render();

    /**
     * @brief Tells whether the game session is still active
     * @return true if the game is running, false if it has ended
     */
    bool isRunning() const;

    // ----------------------------------------------------------
    //  Observer Pattern — ISubject
    // ----------------------------------------------------------
    void subscribe(std::shared_ptr<IObserver> observer) override;
    void unsubscribe(std::shared_ptr<IObserver> observer) override;
    void notifyObservers(GameEvent event, int value = 0) override;

    // ----------------------------------------------------------
    //  Player actions — called from handleEvent()
    // ----------------------------------------------------------

    /**
     * @brief Attempts to place a tower on the clicked cell
     * @param mousePos  Mouse position in pixels
     * @param type      Tower type to place
     */
    void onTowerPlacement(sf::Vector2i mousePos, TowerType type);

    /**
     * @brief Opens the upgrade menu for the clicked tower
     * @param mousePos  Mouse position in pixels
     */
    void onTowerUpgrade(sf::Vector2i mousePos);

    /**
     * @brief Changes the currently selected tower type in the palette
     * @param type  Newly selected type
     */
    void onTowerTypeSelected(TowerType type);

    /**
     * @brief Manually triggers the start of the next wave
     *        (if the preparation timer has not yet expired)
     */
    void onStartWave();

    /**
     * @brief Toggles the game pause state
     */
    void onTogglePause();

    /**
     * @brief Exits the current game session and returns to the main menu
     */
    void onQuit();

private:
    // ----------------------------------------------------------
    //  References to the other MVC layers
    // ----------------------------------------------------------
    sf::RenderWindow&          m_window;
    std::shared_ptr<Game>      m_game;      ///< Model (data)
    std::shared_ptr<GameView>  m_gameView;  ///< View (rendering)

    // ----------------------------------------------------------
    //  Dedicated wave sub-controller
    // ----------------------------------------------------------
    std::unique_ptr<WaveController> m_waveController;

    // ----------------------------------------------------------
    //  Internal controller state
    // ----------------------------------------------------------
    TowerType m_selectedTowerType = TowerType::ARCHER; ///< Active tower in the palette
    bool      m_isPaused          = false;             ///< Is the game paused?
    bool      m_isRunning         = true;              ///< Is the session active?

    // ----------------------------------------------------------
    //  Private helper methods
    // ----------------------------------------------------------

    /**
     * @brief Converts a pixel position to grid coordinates
     * @param mousePos  Position in pixels
     * @return Grid coordinates (column, row)
     */
    sf::Vector2i pixelToGrid(sf::Vector2i mousePos) const;

    /**
     * @brief Checks for dead enemies and distributes gold + score
     */
    void checkEnemyDeaths();

    /**
     * @brief Checks whether any enemy has reached the base
     */
    void checkBaseReached();

    /**
     * @brief Checks win and loss conditions
     */
    void checkGameOver();
};