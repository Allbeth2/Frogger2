#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "gameState.h"
#include <vector>
#include <list>
#include <random>
#include <fstream> // For fstream in LoadEntitiesFromFile
#include "Vector2D.h" // For Point2D
#include "FileNotFoundError.h" // For exceptions

// Forward declarations of game objects that PlayState will manage
class Frog;
class Vehicle;
class Log;
class HomedFrog;
class Wasp;
class Collision;
class SceneObject;

class PlayState : public GameState {
public:
    PlayState(Game* game);
    ~PlayState();

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

    // Game-specific methods from old Game class
    Collision checkCollision(const SDL_FRect& rect);
    Collision nestChecking(const SDL_FRect& rect);
    int getRandomRange(int min, int max);
    void deleteAfter(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO);

private:
    std::list<SceneObject*> sceneObjectsForCollision_; // Additional list for SceneObjects

    Frog* frogPointer_;
    std::vector<SDL_FRect> nests_;
    std::mt19937_64 randomGenerator_;

    bool allNestsOccupied() const;
    void trySpawnWasp();
    bool LoadEntitiesFromFile();
    void processEntity(char entidad, std::fstream& file, int lineNumber);

    // Overloaded versions that read from file (these were present in original Game.cpp)
    void loadVehicle(std::fstream& file, int lineNumber);
    void loadLog(std::fstream& file, int lineNumber);
    void loadTurtleGroup(std::fstream& file, int lineNumber);
    void loadFrog(std::fstream& file, int lineNumber);
};

#endif // PLAY_STATE_H