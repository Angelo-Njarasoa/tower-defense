#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include "../patterns/Observer.hpp"

class SoundManager : public IObserver {
public:
    SoundManager();
    void onNotify(GameEvent event, int value = 0) override;

private:
    enum SoundId { PLACE, UPGRADE, ENEMY_DEAD, BASE_HIT, WAVE_START, WAVE_CLEAR, GAME_OVER, GAME_WIN, COUNT };

    std::array<sf::SoundBuffer, COUNT> m_buffers;
    std::array<sf::Sound,       COUNT> m_sounds;

    static std::vector<sf::Int16> makeTone(float freq, float dur, float vol = 0.55f);
    static std::vector<sf::Int16> makeSlide(float f0, float f1, float dur, float vol = 0.55f);
    static std::vector<sf::Int16> makeChord(std::vector<float> freqs, float dur, float vol = 0.45f);
    static std::vector<sf::Int16> makeArpeggio(std::vector<float> freqs, float noteDur, float vol = 0.55f);
};
