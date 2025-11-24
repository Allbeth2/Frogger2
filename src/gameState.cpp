#include "gameState.h"
#include "GameObject.h" 
#include "EventHandler.h"


GameState::~GameState() {
    for (GameObject* obj : gameObjects_) {
        delete obj;
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
