#include "../../include/view/SoundManager.hpp"
#include <cmath>
#include <algorithm>

static constexpr int   RATE = 44100;
static constexpr float PI2  = 6.28318530f;

// Single sustained tone with short attack/release envelope
std::vector<sf::Int16> SoundManager::makeTone(float freq, float dur, float vol) {
    int n = static_cast<int>(RATE * dur);
    std::vector<sf::Int16> s(n);
    for (int i = 0; i < n; ++i) {
        float t   = (float)i / RATE;
        float env = std::min(1.f, std::min(t / 0.01f, (dur - t) / 0.04f));
        s[i] = static_cast<sf::Int16>(32767.f * vol * env * std::sin(PI2 * freq * t));
    }
    return s;
}

// Tone that glides from f0 to f1 (pitch bend)
std::vector<sf::Int16> SoundManager::makeSlide(float f0, float f1, float dur, float vol) {
    int n = static_cast<int>(RATE * dur);
    std::vector<sf::Int16> s(n);
    float phase = 0.f;
    for (int i = 0; i < n; ++i) {
        float t    = (float)i / RATE;
        float freq = f0 + (f1 - f0) * (t / dur);
        float env  = std::min(1.f, std::min(t / 0.01f, (dur - t) / 0.04f));
        phase += PI2 * freq / RATE;
        s[i] = static_cast<sf::Int16>(32767.f * vol * env * std::sin(phase));
    }
    return s;
}

// Several frequencies played together
std::vector<sf::Int16> SoundManager::makeChord(std::vector<float> freqs, float dur, float vol) {
    int n = static_cast<int>(RATE * dur);
    std::vector<float> accum(n, 0.f);
    float v = vol / static_cast<float>(freqs.size());
    for (float freq : freqs) {
        for (int i = 0; i < n; ++i) {
            float t   = (float)i / RATE;
            float env = std::min(1.f, std::min(t / 0.02f, (dur - t) / 0.1f));
            accum[i] += v * env * std::sin(PI2 * freq * t);
        }
    }
    std::vector<sf::Int16> s(n);
    for (int i = 0; i < n; ++i)
        s[i] = static_cast<sf::Int16>(32767.f * std::max(-1.f, std::min(1.f, accum[i])));
    return s;
}

// Notes played one after another
std::vector<sf::Int16> SoundManager::makeArpeggio(std::vector<float> freqs, float noteDur, float vol) {
    std::vector<sf::Int16> result;
    for (float f : freqs) {
        auto note = makeTone(f, noteDur, vol);
        result.insert(result.end(), note.begin(), note.end());
    }
    return result;
}

SoundManager::SoundManager() {
    struct Entry { SoundId id; std::vector<sf::Int16> samples; };
    std::vector<Entry> entries = {
        { PLACE,      makeTone(800.f, 0.08f, 0.45f) },
        { UPGRADE,    makeArpeggio({800.f, 1200.f}, 0.07f, 0.45f) },
        { ENEMY_DEAD, makeSlide(480.f, 120.f, 0.12f) },
        { BASE_HIT,   makeTone(90.f,  0.22f, 0.75f) },
        { WAVE_START, makeSlide(350.f, 700.f, 0.28f) },
        { WAVE_CLEAR, makeChord({523.f, 659.f, 784.f}, 0.55f) },
        { GAME_OVER,  makeSlide(440.f, 80.f,  1.0f,  0.65f) },
        { GAME_WIN,   makeArpeggio({523.f, 659.f, 784.f, 1047.f}, 0.22f, 0.55f) },
    };

    for (auto& e : entries) {
        m_buffers[e.id].loadFromSamples(e.samples.data(), e.samples.size(), 1, RATE);
        m_sounds[e.id].setBuffer(m_buffers[e.id]);
    }
}

void SoundManager::onNotify(GameEvent event, int /*value*/) {
    switch (event) {
        case GameEvent::TOWER_PLACED:       m_sounds[PLACE].play();      break;
        case GameEvent::TOWER_UPGRADED:     m_sounds[UPGRADE].play();    break;
        case GameEvent::ENEMY_DEAD:         m_sounds[ENEMY_DEAD].play(); break;
        case GameEvent::ENEMY_REACHED_BASE: m_sounds[BASE_HIT].play();   break;
        case GameEvent::WAVE_STARTED:       m_sounds[WAVE_START].play(); break;
        case GameEvent::WAVE_CLEARED:       m_sounds[WAVE_CLEAR].play(); break;
        case GameEvent::GAME_OVER:          m_sounds[GAME_OVER].play();  break;
        case GameEvent::GAME_WIN:           m_sounds[GAME_WIN].play();   break;
        default: break;
    }
}
