#include "gameState.h"
#include "GameObject.h" // Assuming GameObject.h exists and defines GameObject
#include "EventHandler.h" // Now EventHandler.h exists

// No need to include Game.h here, as GameState only holds a Game* pointer and doesn't directly use Game's methods
// #include "Game.h"

GameState::~GameState() {
    for (GameObject* obj : gameObjects_) {
        delete obj;
    }
    // As per enunPract3.pdf, GameState is owner of GameObject instances.
    // EventHandler objects will be managed by their respective owners (e.g., Button, Frog)
    // and will typically not be deleted by GameState directly.
    // If GameState *does* own them, they should be deleted here.
    // For now, let's assume GameState does not own EventHandler objects.
    // The same applies to DelayedCallbacks, they are functions not objects to be deleted.
}

void GameState::addEventListener(EventHandler* handler) {
    eventHandlers_.push_back(handler);
}

void GameState::addObject(GameObject* obj) {
    gameObjects_.push_back(obj);
}

void GameState::runLater(DelayedCallback cb) {
    delayedCallbacks_.push_back(std::move(cb));
}