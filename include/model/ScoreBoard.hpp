#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Scoreboard
{
private:
    int score;
    int wave;
    int lives;

    sf::Font font;
    sf::Text scoreText;
    sf::Text waveText;
    sf::Text livesText;

    void updateText();

public:
    Scoreboard();

    bool loadFont(const std::string& fontPath);

    void addScore(int value);
    void nextWave();
    void loseLife(int value);

    void draw(sf::RenderWindow& window);
};

#endif