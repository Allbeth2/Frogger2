#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <memory>    
#include <functional> 
#include <SDL3/SDL.h>


class Game;
class GameObject;
class EventHandler;

class GameState {
public:
    using DelayedCallback = std::function<void()>;
    using Anchor = std::list<GameObject*>::iterator;


    GameState(Game* game) : game_(game) {}
    virtual ~GameState();

    virtual void update();
    virtual void render() const;
    virtual void handleEvent(const SDL_Event& event) = 0;

    void addEventListener(EventHandler* handler);
    Anchor addObject(GameObject* obj);
    void runLater(DelayedCallback cb);

    Game* getGame() const { return game_; }

protected:
    Game* game_;
    std::list<GameObject*> gameObjects_;
    std::list<EventHandler*> eventHandlers_;
    std::list<DelayedCallback> delayedCallbacks_;
};

#endif // GAME_STATE_H
