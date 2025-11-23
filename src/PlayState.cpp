#include "PlayState.h"
#include "EventHandler.h" // Include EventHandler.h
#include <ctime>
#include <algorithm>
#include <fstream>
#include <limits>
#include "Game.h" // Needed for Game::TextureName and access to textures
#include "Texture.h" // No longer directly used here, but for completeness with original Game.cpp
#include "Frog.h"
#include "HomedFrog.h"
#include "Vehicle.h"
#include "Log.h"
#include "TurtleGroup.h"
#include "Wasp.h"
#include "SceneObject.h"
#include "Collision.h"
#include "FileFormatError.h"
#include "FileNotFoundError.h" // Explicitly include this, as it's used

using namespace std;

// Default map file for initial PlayState (if not specified)
constexpr const char* const DEFAULT_MAP_FILE = "../assets/maps/Original.txt";

PlayState::PlayState(Game* game)
    : PlayState(game, DEFAULT_MAP_FILE) // Delegate to the new constructor
{
}

PlayState::PlayState(Game* game, const std::string& mapFilePath)
    : GameState(game), // Initialize base class
    mapFilePath_(mapFilePath), // Initialize map file path
    frogPointer_(nullptr),
    randomGenerator_(time(nullptr))
{
    // Nests initialization (from original Game.cpp)
    constexpr float nido0x = 16;
    constexpr float nido0y = 22;
    constexpr float HorizontalGap = 96;

    for (int i = 0; i < 5; ++i) {
        nests_.push_back(SDL_FRect{
            nido0x + (i * HorizontalGap),
            nido0y,
            (float)Game::CellSize, // Use Game's static constexpr
            (float)Game::CellSize - 1 // Use Game's static constexpr
            });
    }

    // Load entities (from original Game.cpp)
    LoadEntitiesFromFile();
}

PlayState::~PlayState()
{
    // Clean up sceneObjectsForCollision_
    for(SceneObject* ele : sceneObjectsForCollision_)
    {
        // Check if the object is also in gameObjects_ (base class)
        // If it is, GameState's destructor will delete it.
        // If it's *only* in sceneObjectsForCollision_ (shouldn't happen if added correctly), delete here.
        // For now, assume objects added to sceneObjectsForCollision_ are also added to gameObjects_
        // So, no explicit deletion here, as GameState's destructor will handle GameObject* deletion.
        // Just clear the list.
    }
    sceneObjectsForCollision_.clear();
    frogPointer_ = nullptr; // Frog object is owned by gameObjects_ in GameState
}

#include "EndState.h" // Include EndState.h

void PlayState::update()
{
    // Update gameObjects_ (inherited from GameState)
    for (GameObject* obj : gameObjects_)
    {
        obj->update();
    }

    trySpawnWasp();

    // Check game over conditions
    if (frogPointer_ != nullptr)
    {
        if (frogPointer_->getLives() <= 0)
        {
            game_->pushState(std::make_shared<EndState>(game_, false)); // Lost
        }
        else if (allNestsOccupied())
        {
            game_->pushState(std::make_shared<EndState>(game_, true)); // Won
        }
    }

    // Execute Delayed Callbacks
    for (const auto& cb : delayedCallbacks_) {
        cb();
    }
    delayedCallbacks_.clear(); // Clear after executing
}

void PlayState::render() const
{
    game_->getTexture(Game::BACKGROUND)->render(); // Render background

    // Render inherited gameObjects_
    for (GameObject* obj : gameObjects_)
    {
        obj->render();
    }
}

#include "PauseState.h"

void PlayState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.key == SDLK_ESCAPE)
        {
            game_->pushState(std::make_shared<PauseState>(game_, this));
        }
        else if (frogPointer_)
        {
            frogPointer_->handleEvent(event);
        }
    }
    // Also pass event to registered eventHandlers_ (inherited from GameState)
    for (EventHandler* handler : eventHandlers_) {
        handler->handleEvent(event);
    }
}

Collision PlayState::checkCollision(const SDL_FRect& frogRect)
{
    // (Content from original Game::checkCollision)
    if (frogRect.y < Game::nestHeight)
    {
        return nestChecking(frogRect);
    }

    Collision result(Collision::Type::NONE, Vector2D<float>(0, 0));

    for (SceneObject* obj : sceneObjectsForCollision_)
    {
        if (obj != frogPointer_) // Don't collide with self
        {
            Collision currentCollision = obj->checkCollision(frogRect);
            if (currentCollision.collisionType == Collision::Type::ENEMY) {
                return currentCollision; // Enemy collision alta prioridad
            }
            if (currentCollision.collisionType == Collision::Type::PLATFORM) {
                result = currentCollision;
            }
        }
    }

    return result;
}

Collision PlayState::nestChecking (const SDL_FRect& frogRect)
{
    // (Content from original Game::nestChecking)
    SDL_FRect* nestFound = nullptr;
    for (SDL_FRect& nest : nests_) {
        if (SDL_HasRectIntersectionFloat(&frogRect, &nest)) {
            nestFound = &nest;
            break;
        }
    }
    if (nestFound != nullptr)
    {
        Point2D<float> nestPos(nestFound->x, nestFound->y);
        for (SceneObject* obj : sceneObjectsForCollision_) {
            if (obj->isHomedFrog()) {
                if (obj->getBoundingBox().x == nestPos.getX() && obj->getBoundingBox().y == nestPos.getY()) {
                    return Collision(Collision::ENEMY, Vector2D<float>(0,0));
                }
            }
        }

        // Use addObjects from GameState to add HomedFrog
        HomedFrog* homedFrog = new HomedFrog(this, game_->getTexture(Game::TextureName::FROG), nestPos);
        addObject(homedFrog); // Adds to gameObjects_ 
        sceneObjectsForCollision_.push_back(homedFrog); // Also add to collision list
        return Collision(Collision::NONE, Vector2D<float>(0,0));
    }
    return Collision(Collision::ENEMY, Vector2D<float>(0,0));
}

int PlayState::getRandomRange(int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(randomGenerator_);
}

void PlayState::deleteAfter(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO)
{
    // This method is called from Wasp when it needs to be deleted.
    // It should register a DelayedCallback to safely remove the Wasp.
    runLater([this, itGO, itSCO]() {
        // Get the pointer before erasing
        GameObject* objToDelete = *itGO; 

        // Erase from both lists
        gameObjects_.erase(itGO);
        sceneObjectsForCollision_.erase(itSCO);

        // Delete the object
        delete objToDelete;
    });
}

void PlayState::trySpawnWasp()
{
    // (Content from original Game::trySpawnWasp)
    int chance = getRandomRange(0, 200);
    if (chance < Game::waspSpawnChance) {
        int nestIndex = getRandomRange(0, nests_.size() - 1);
        SDL_FRect& chosenNest = nests_[nestIndex];
        Point2D<float> nestPos(chosenNest.x, chosenNest.y);

        bool occupied = false;
        for (SceneObject* so : sceneObjectsForCollision_) {
            if (so->isHomedFrog()) {
                if (so->getBoundingBox().x == nestPos.getX() && so->getBoundingBox().y == nestPos.getY()) {
                    occupied = true;
                    break;
                }
            }
        }

        if (!occupied) {
            bool waspExists = false;
            for (SceneObject* so : sceneObjectsForCollision_) {
                if (so->isWasp()) {
                    waspExists = true;
                    break;
                }
            }
            if(!waspExists)
            {
                Uint32 lifetime = getRandomRange(Game::minWaspLifetime, Game::maxWaspLifetime);
                Wasp* newWasp = new Wasp(this, game_->getTexture(Game::TextureName::WASP), nestPos, Vector2D<float>(0, 0), lifetime);
                
                // Add to both lists and get iterators
                auto itGO = gameObjects_.insert(gameObjects_.end(), newWasp);
                auto itSCO = sceneObjectsForCollision_.insert(sceneObjectsForCollision_.end(), newWasp);
                
                // Set the iterators in the Wasp object
                newWasp->setIterators(itGO, itSCO);
            }
        }
    }
}

bool PlayState::allNestsOccupied() const
{
    // (Content from original Game::allNestsOccupied)
    int homedFrogCount = 0;
    for(SceneObject* so : sceneObjectsForCollision_)
    {
        if(so->isHomedFrog())
        {
            homedFrogCount++;
        }
    }
    return homedFrogCount == nests_.size();
}

bool PlayState::LoadEntitiesFromFile()
{
    std::fstream file(mapFilePath_, std::ios::in);
    if (!file.is_open())
    {
        throw FileNotFoundError("Map file not found: "s + mapFilePath_);
        return false;
    }

    char entidad;
    int lineNumber = 0;

    while (file >> entidad)
    {
        lineNumber++;
        processEntity(entidad, file, lineNumber);
    }

    return true;
}

void PlayState::loadWasp(std::fstream& file, int lineNumber)
{
    Wasp* newWasp = new Wasp(this, file, lineNumber);
    
    // Add to both lists and get iterators
    auto itGO = gameObjects_.insert(gameObjects_.end(), newWasp);
    auto itSCO = sceneObjectsForCollision_.insert(sceneObjectsForCollision_.end(), newWasp);
    
    // Set the iterators in the Wasp object
    newWasp->setIterators(itGO, itSCO);
}

void PlayState::processEntity(char entidad, std::fstream& file, int lineNumber)
{
    // (Content from original Game::processEntity)
    switch (entidad) {
    case 'V':
        loadVehicle(file, lineNumber);
        break;
    case 'L':
        loadLog(file, lineNumber);
        break;
    case 'T':
        loadTurtleGroup(file, lineNumber);
        break;
    case 'F':
        loadFrog(file, lineNumber);
        break;
    case 'W':
        loadWasp(file, lineNumber);
        break;
    default:
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        break;
    }
}

void PlayState::loadVehicle(std::fstream& file, int lineNumber)
{
    Vehicle* newVehicle = new Vehicle(this, file, lineNumber);
    addObject(newVehicle); // Add to GameState's gameObjects_
    sceneObjectsForCollision_.push_back(newVehicle); // Add to collision list
}

void PlayState::loadLog(std::fstream& file, int lineNumber)
{
    Log* newLog = new Log(this, file, lineNumber);
    addObject(newLog);
    sceneObjectsForCollision_.push_back(newLog);
}

void PlayState::loadTurtleGroup(std::fstream& file, int lineNumber)
{
    TurtleGroup* newTurtleGroup = new TurtleGroup(this, file, lineNumber);
    addObject(newTurtleGroup);
    sceneObjectsForCollision_.push_back(newTurtleGroup);
}

void PlayState::loadFrog(std::fstream& file, int lineNumber)
{
    Frog* newFrog = new Frog(this, file, lineNumber);
    addObject(newFrog);
    sceneObjectsForCollision_.push_back(newFrog);
    frogPointer_ = newFrog;
}
