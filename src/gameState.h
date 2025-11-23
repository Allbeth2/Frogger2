#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <memory>    // For shared_ptr if needed
#include <functional> // For std::function
#include <SDL3/SDL.h>     // For SDL_Event

// Forward declarations
class Game;
class GameObject;
class EventHandler; // Already created src/EventHandler.h

class GameState {
public:
    using DelayedCallback = std::function<void()>;

    GameState(Game* game) : game_(game) {}
    virtual ~GameState();

    virtual void update() = 0;
    virtual void render() const = 0;
    virtual void handleEvent(const SDL_Event& event) = 0;

    void addEventListener(EventHandler* handler);
    void addObject(GameObject* obj);
    void runLater(DelayedCallback cb);

    Game* getGame() const { return game_; }

protected:
    Game* game_;
    std::list<GameObject*> gameObjects_;
    std::list<EventHandler*> eventHandlers_;
    std::list<DelayedCallback> delayedCallbacks_;
};

#endif // GAME_STATE_H