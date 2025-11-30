#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "gameState.h"
#include <vector>
#include <list>
#include <filesystem>
#include <random>
#include <fstream> 
#include "Vector2D.h" 
#include "FileNotFoundError.h" 

class Frog;
class Vehicle;
class Log;
class HomedFrog;
class Wasp;
struct Collision;
class SceneObject;

class PlayState : public GameState {
public:
    using Anchor = std::list<SceneObject*>::iterator;
    
    PlayState(Game* game);
    PlayState(Game* game, const std::filesystem::path& mapFilePath);
    ~PlayState();

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

    /**
     * @brief Añade un SceneObject a la lista de colisiones.
     * @param obj El SceneObject a añadir.
     * @return Un iterador al objeto recién añadido.
     */
    Anchor addObject(SceneObject* obj);

    // Métodos específicos del juego de la antigua clase Game
    /**
     * @brief Comprueba si hay colisión con algún objeto de la escena.
     * @param rect El rectángulo para comprobar la colisión.
     * @return Un objeto Collision con los detalles de la colisión.
     */
    Collision checkCollision(const SDL_FRect& rect);

    /**
     * @brief Comprueba si hay colisión con los nidos.
     * @param rect El rectángulo para comprobar la colisión.
     * @return Un objeto Collision con los detalles de la colisión.
     */
    Collision nestChecking(const SDL_FRect& rect);

    /**
     * @brief Genera un número aleatorio en un rango dado.
     * @param min El valor mínimo del rango.
     * @param max El valor máximo del rango.
     * @return Un entero aleatorio.
     */
    int getRandomRange(int min, int max);

    /**
     * @brief Elimina un objeto de juego después del ciclo de actualización actual.
     * @param itGO Iterador al objeto en la lista gameObjects_.
     * @param itSCO Iterador al objeto en la lista sceneObjectsForCollision_.
     */
    void deleteAfter(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO);    

    /**
     * @brief Obtiene la ruta del archivo del mapa actual.
     * @return La ruta del archivo del mapa.
     */
    const std::filesystem::path& getMapFilePath() const { return mapFilePath_; }

private:
    std::filesystem::path mapFilePath_;
    std::list<SceneObject*> sceneObjectsForCollision_; // Lista adicional para colisiones

    Frog* frogPointer_;
    std::vector<SDL_FRect> nests_;
    std::mt19937_64 randomGenerator_;

    /**
     * @brief Comprueba si todos los nidos están ocupados por ranas.
     * @return True si todos los nidos están ocupados, false en caso contrario.
     */
    bool allNestsOccupied() const;

    /**
     * @brief Comprueba si un nido en una posición dada está ocupado.
     * @param pos La posición del nido a comprobar.
     * @return True si el nido está ocupado, false en caso contrario.
     */
    bool isNestOccupied(const Point2D<float>& pos) const;

    /**
     * @brief Intenta generar una avispa en un nido vacío aleatorio.
     */
    void trySpawnWasp();

    /**
     * @brief Carga todas las entidades desde el archivo de mapa.
     * @return True si el archivo se cargó correctamente, false en caso contrario.
     */
    bool LoadEntitiesFromFile();

    /**
     * @brief Procesa una única entidad del archivo de mapa.
     * @param entidad El carácter que representa la entidad.
     * @param file El stream del archivo para leer.
     * @param lineNumber El número de línea actual en el archivo.
     */
    void processEntity(char entidad, std::fstream& file, int lineNumber);

    void loadVehicle(std::fstream& file, int lineNumber);
    void loadLog(std::fstream& file, int lineNumber);
    void loadTurtleGroup(std::fstream& file, int lineNumber);
    void loadFrog(std::fstream& file, int lineNumber);
    void loadWasp(std::fstream& file, int lineNumber);
};

#endif // PLAY_STATE_H
