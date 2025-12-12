#include "gameState.h"
#include "GameObject.h" 
#include "EventHandler.h"


GameState::~GameState() {
    for (GameObject* obj : gameObjects_) {
        delete obj;
    }

}

void GameState::update()
{
    for (auto& obj : gameObjects_)
    {
        obj->update();
    }
}

void GameState::render() const
{
    for (const auto& obj : gameObjects_)
    {
        obj->render();
    }
}

void GameState::addEventListener(EventHandler* handler) {
    eventHandlers_.push_back(handler);
}

GameState::Anchor GameState::addObject(GameObject* obj) {
    gameObjects_.push_back(obj);
    return std::prev(gameObjects_.end());
}

void GameState::runLater(DelayedCallback cb) {
    delayedCallbacks_.push_back(std::move(cb));
}
