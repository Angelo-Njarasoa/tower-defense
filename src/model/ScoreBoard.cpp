#include "Scoreboard.hpp"
#include <iostream>

Scoreboard::Scoreboard()
{
    score = 0;
    wave = 1;
    lives = 10;
}

bool Scoreboard::loadFont(const std::string& fontPath)
{
    if (!font.loadFromFile(fontPath))
    {
        std::cerr << "Erreur chargement font" << std::endl;
        return false;
    }

    scoreText.setFont(font);
    waveText.setFont(font);
    livesText.setFont(font);

    scoreText.setCharacterSize(20);
    waveText.setCharacterSize(20);
    livesText.setCharacterSize(20);

    scoreText.setPosition(10.f, 10.f);
    waveText.setPosition(10.f, 40.f);
    livesText.setPosition(10.f, 70.f);

    scoreText.setFillColor(sf::Color::White);
    waveText.setFillColor(sf::Color::White);
    livesText.setFillColor(sf::Color::White);

    updateText();

    return true;
}

void Scoreboard::updateText()
{
    scoreText.setString("Score: " + std::to_string(score));
    waveText.setString("Wave: " + std::to_string(wave));
    livesText.setString("Lives: " + std::to_string(lives));
}

void Scoreboard::addScore(int value)
{
    score += value;
    updateText();
}

void Scoreboard::nextWave()
{
    wave++;
    updateText();
}

void Scoreboard::loseLife(int value)
{
    lives -= value;
    if (lives < 0) lives = 0;
    updateText();
}

void Scoreboard::draw(sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(waveText);
    window.draw(livesText);
}