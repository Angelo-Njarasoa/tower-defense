#pragma once

#include <vector>
#include <memory>
#include <algorithm>

//  All game events

enum class GameEvent {
    ENEMY_DEAD,         // An enemy was killed
    ENEMY_REACHED_BASE, // An enemy reached the base
    TOWER_PLACED,       // A tower was placed on the grid
    TOWER_UPGRADED,     // A tower was upgraded
    WAVE_STARTED,       // A new wave has begun
    WAVE_CLEARED,       // A wave was fully eliminated
    GAME_OVER,          // Player lost (base HP = 0)
    GAME_WIN,           // Player won (all waves cleared)
    GOLD_CHANGED,       // Gold amount changed
    SCORE_CHANGED,      // Score changed
};

//  IObserver interface
//  Any class that wants to be notified must implement onNotify()

class IObserver {
public:
    virtual ~IObserver() = default;

    /**
     * @brief Called by the subject when an event occurs
     * @param event  Type of event that occurred
     * @param value  Optional value associated with the event
     *               (e.g. gold earned, damage dealt...)
     */
    virtual void onNotify(GameEvent event, int value = 0) = 0;
};

//  ISubject interface
//  Any class that emits events must implement these methods

class ISubject {
public:
    virtual ~ISubject() = default;

    /**
     * @brief Subscribes an observer to the subject's events
     * @param observer  Shared pointer to the observer
     */
    virtual void subscribe(std::shared_ptr<IObserver> observer) = 0;

    /**
     * @brief Unsubscribes an observer
     * @param observer  Shared pointer to the observer to remove
     */
    virtual void unsubscribe(std::shared_ptr<IObserver> observer) = 0;

    /**
     * @brief Notifies all subscribed observers
     * @param event  Event to broadcast
     * @param value  Optional associated value
     */
    virtual void notifyObservers(GameEvent event, int value = 0) = 0;

protected:
    // Observers stored as weak_ptr to avoid ownership cycles:
    // the subject does not keep observers alive.
    std::vector<std::weak_ptr<IObserver>> m_observers;
};