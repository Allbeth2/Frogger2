#include "PlayState.h"
#include "EventHandler.h"
#include <ctime>
#include <algorithm>
#include <fstream>
#include <limits>
#include "Game.h" 
#include "Texture.h" 
#include "Frog.h"
#include "HomedFrog.h"
#include "Vehicle.h"
#include "Log.h"
#include "TurtleGroup.h"
#include "Wasp.h"
#include "SceneObject.h"
#include "Collision.h"
#include "FileFormatError.h"
#include "FileNotFoundError.h" 
#include "EndState.h"

using namespace std;

const std::filesystem::path DEFAULT_MAP_FILE = "../assets/maps/Original.txt";

PlayState::PlayState(Game* game)
    : PlayState(game, DEFAULT_MAP_FILE)
{
}

PlayState::PlayState(Game* game, const std::filesystem::path& mapFilePath)
	: GameState(game), // Inicializa la clase base GameState
	mapFilePath_(mapFilePath), // Inicializa la ruta del archivo del mapa 
    frogPointer_(nullptr),
    randomGenerator_(time(nullptr))
{
	// Inicializa los nidos
    constexpr float nido0x = 16;
    constexpr float nido0y = 22;
    constexpr float HorizontalGap = 96;

    for (int i = 0; i < 5; ++i) {
        nests_.push_back(SDL_FRect{
            nido0x + (i * HorizontalGap),
            nido0y,
            (float)Game::CellSize, 
            (float)Game::CellSize - 1
            });
    }

    LoadEntitiesFromFile();
}

PlayState::~PlayState()
{
    //for(SceneObject* ele : sceneObjectsForCollision_)
    //{
    //}
    sceneObjectsForCollision_.clear();
    frogPointer_ = nullptr;
}


void PlayState::update()
{
    for (GameObject* obj : gameObjects_)
    {
        obj->update();
    }

    trySpawnWasp();

    if (frogPointer_ != nullptr)
    {
        if (frogPointer_->getLives() <= 0)
        {
			game_->pushState(std::make_shared<EndState>(game_, false)); // El juego pierde todas las vidas
        }
        else if (allNestsOccupied())
        {
			game_->pushState(std::make_shared<EndState>(game_, true)); // El jugador gana al ocupar todos los nidos
        }
    }

	// Se ejecutan los callbacks retrasados
    for (const auto& cb : delayedCallbacks_) {
        cb();
    }
    delayedCallbacks_.clear();
}

void PlayState::render() const
{
	game_->getTexture(Game::BACKGROUND)->render(); // Se renderiza el fondo primero


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
    }

    for (EventHandler* handler : eventHandlers_) {
        handler->handleEvent(event);
    }
}

Collision PlayState::checkCollision(const SDL_FRect& frogRect)
{

    if (frogRect.y < Game::nestHeight)
    {
        return nestChecking(frogRect);
    }

    Collision result(Collision::Type::NONE, Vector2D<float>(0, 0));

    for (SceneObject* obj : sceneObjectsForCollision_)
    {
		if (obj != frogPointer_) // Para que la rana no colisione consigo misma
        {
            Collision currentCollision = obj->checkCollision(frogRect);
            if (currentCollision.collisionType == Collision::Type::ENEMY) {
                return currentCollision;
            }
            if (currentCollision.collisionType == Collision::Type::PLATFORM) {
                result = currentCollision;
            }
        }
    }

    return result;
}

/**
 * @brief Añade un SceneObject a la lista de colisiones.
 * @param obj El SceneObject a añadir.
 * @return Un iterador al objeto recién añadido.
 */
PlayState::Anchor PlayState::addObject(SceneObject* obj)
{
    sceneObjectsForCollision_.push_back(obj);
    return std::prev(sceneObjectsForCollision_.end());
}

/**
 * @brief Comprueba si hay colisión con los nidos.
 * @param rect El rectángulo para comprobar la colisión.
 * @return Un objeto Collision con los detalles de la colisión.
 */
Collision PlayState::nestChecking (const SDL_FRect& frogRect)
{

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


        HomedFrog* homedFrog = new HomedFrog(this, game_->getTexture(Game::TextureName::FROG), nestPos);
        GameState::addObject(homedFrog);
        addObject(homedFrog);
        return Collision(Collision::NONE, Vector2D<float>(0,0));
    }
    return Collision(Collision::ENEMY, Vector2D<float>(0,0));
}

/**
 * @brief Genera un número aleatorio en un rango dado.
 * @param min El valor mínimo del rango.
 * @param max El valor máximo del rango.
 * @return Un entero aleatorio.
 */
int PlayState::getRandomRange(int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(randomGenerator_);
}

/**
 * @brief Elimina un objeto de juego después del ciclo de actualización actual.
 * @param itGO Iterador al objeto en la lista gameObjects_.
 * @param itSCO Iterador al objeto en la lista sceneObjectsForCollision_.
 */
void PlayState::deleteAfter(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO)
{
    // Este método es llamado desde Wasp cuando necesita ser eliminado.
    // Registra un DelayedCallback para eliminar la avispa de forma segura.
    runLater([this, itGO, itSCO]() {
        // Obtiene el puntero antes de borrar
        GameObject* objToDelete = *itGO; 

        // Borra de ambas listas
        gameObjects_.erase(itGO);
        sceneObjectsForCollision_.erase(itSCO);

        // Elimina el objeto
        delete objToDelete;
    });
}

/**
 * @brief Comprueba si un nido en una posición dada está ocupado.
 * @param pos La posición del nido a comprobar.
 * @return True si el nido está ocupado, false en caso contrario.
 */
bool PlayState::isNestOccupied(const Point2D<float>& pos) const
{
    for (SceneObject* so : sceneObjectsForCollision_) {
        if (so->isHomedFrog()) {
            if (so->getBoundingBox().x == pos.getX() && so->getBoundingBox().y == pos.getY()) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Intenta generar una avispa en un nido vacío aleatorio.
 */
void PlayState::trySpawnWasp()
{

    int chance = getRandomRange(0, 200);
    if (chance < Game::waspSpawnChance) {
        int nestIndex = getRandomRange(0, nests_.size() - 1);
        SDL_FRect& chosenNest = nests_[nestIndex];
        Point2D<float> nestPos(chosenNest.x, chosenNest.y);

        if (!isNestOccupied(nestPos)) {
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
                
                // Añade a ambas listas y obtiene los iteradores
                auto itGO = GameState::addObject(newWasp);
                auto itSCO = addObject(newWasp);
                
                // Establece los iteradores en el objeto Wasp
                newWasp->setIterators(itGO, itSCO);
            }
        }
    }
}

/**
 * @brief Comprueba si todos los nidos están ocupados por ranas.
 * @return True si todos los nidos están ocupados, false en caso contrario.
 */
bool PlayState::allNestsOccupied() const
{

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

/**
 * @brief Carga todas las entidades desde el archivo de mapa.
 * @return True si el archivo se cargó correctamente, false en caso contrario.
 */
bool PlayState::LoadEntitiesFromFile()
{
    std::fstream file(mapFilePath_);
    if (!file.is_open())
    {
        throw FileNotFoundError("Map file not found: "s + mapFilePath_.string());
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

/**
 * @brief Carga una avispa desde el archivo.
 */
void PlayState::loadWasp(std::fstream& file, int lineNumber)
{
    Wasp* newWasp = new Wasp(this, file, lineNumber);
    
    // Añade a ambas listas y obtiene los iteradores
    auto itGO = GameState::addObject(newWasp);
    auto itSCO = addObject(newWasp);
    
    // Establece los iteradores en el objeto Wasp
    newWasp->setIterators(itGO, itSCO);
}

/**
 * @brief Procesa una única entidad del archivo de mapa.
 * @param entidad El carácter que representa la entidad.
 * @param file El stream del archivo para leer.
 * @param lineNumber El número de línea actual en el archivo.
 */
void PlayState::processEntity(char entidad, std::fstream& file, int lineNumber)
{

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

/**
 * @brief Carga un vehículo desde el archivo.
 */
void PlayState::loadVehicle(std::fstream& file, int lineNumber)
{
    Vehicle* newVehicle = new Vehicle(this, file, lineNumber);
    GameState::addObject(newVehicle); // Añade a gameObjects_
    addObject(newVehicle); // Añade a la lista de colisiones
}

/**
 * @brief Carga un tronco desde el archivo.
 */
void PlayState::loadLog(std::fstream& file, int lineNumber)
{
    Log* newLog = new Log(this, file, lineNumber);
    GameState::addObject(newLog);
    addObject(newLog);
}

/**
 * @brief Carga un grupo de tortugas desde el archivo.
 */
void PlayState::loadTurtleGroup(std::fstream& file, int lineNumber)
{
    TurtleGroup* newTurtleGroup = new TurtleGroup(this, file, lineNumber);
    GameState::addObject(newTurtleGroup);
    addObject(newTurtleGroup);
}

/**
 * @brief Carga la rana desde el archivo.
 */
void PlayState::loadFrog(std::fstream& file, int lineNumber)
{
    Frog* newFrog = new Frog(this, file, lineNumber);
    GameState::addObject(newFrog);
    addObject(newFrog);
    frogPointer_ = newFrog;
    addEventListener(newFrog); // Añade la rana a los manejadores de eventos
}
