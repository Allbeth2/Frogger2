#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <list>
#include <random>
#include "Vector2D.h"
#include "FileNotFoundError.h"

// Declaraciones anticipadas
class Texture;
class Frog;
class Vehicle;
class Log;
class HomedFrog;
class Wasp;
class Collision;
class SceneObject;


/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Se actualiza el juego cada tantos milisegundos
	static constexpr float FRAME_RATE = 30.0f;
	static constexpr float DELTATIME = FRAME_RATE / 1000.0f;
	static constexpr float DELAYTIME = 1000.0f /FRAME_RATE;

	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 448;
	static constexpr int WINDOW_HEIGHT = 484;

	// Tamaño de las celdas del juego
	static constexpr int CellSize = 32;

	// Los bastidores que permiten a los vehiculos volver a su posicion inicial
	static constexpr int FrameLength = 150;
	static constexpr int leftFrame = -FrameLength;
	static constexpr int rightFrame = WINDOW_WIDTH + FrameLength;

	//Constantes para representar las separaciones entre carretera-agua y agua-nido
	static constexpr int waterHeight = 182;
	static constexpr int nestHeight = 21;

	//Constantes de la avispa
	static constexpr int minWaspLifetime = 3000;
	static constexpr int maxWaspLifetime = 8000;
	static constexpr int waspSpawnChance = 1;

	//constante del ancho del frame de la tortuga
	static constexpr int turtleFrameWidth = 30;



	enum TextureName
	{
		BACKGROUND = 0,
		FROG,
		CAR1, CAR2, CAR3, CAR4, CAR5,
		LOG1, LOG2,
		TURTLE,
		WASP,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	// Lista de los objetos del juego
	std::list<SceneObject*> sceneObjects;

	// Puntero a la rana del jugador
	Frog* frogPointer;

	// Vector de nidos (posiciones donde la rana puede llegar)
	std::vector<SDL_FRect> nests;

	//generador aleatorio
	std::mt19937_64 randomGenerator;

	void render() const;
	void update();
	void handleEvents();

	void trySpawnWasp();

	bool allNestsOccupied() const;

	bool exit;

	// Posición inicial de spawn de la rana
	Point2D<float> frogSpawnPos;

	bool LoadEntitiesFromFile();
	
	// Métodos auxiliares para cargar entidades del archivo
	void processEntity(char entidad, std::fstream& file, int lineNumber);
	void loadVehicle(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int TextureType);
	void loadLog(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int TextureType);
	void loadTurtleGroup(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int turtleCount, bool sink);
	void loadFrog(std::fstream& file, int lineNumber, float Xpos, float Ypos, int FrogLives);

	void loadVehicle(std::fstream& file, int lineNumber);
	void loadLog(std::fstream& file, int lineNumber);
	void loadTurtleGroup(std::fstream& file, int lineNumber);
	void loadFrog(std::fstream& file, int lineNumber);

	//metood para reinicio, limpia la lista
	void cleanUp();

	//metodo para mostrar la message box en pantalla
	void messageBox();

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Obtiene el renderer del juego
	SDL_Renderer* getRenderer();

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	Collision checkCollision(const SDL_FRect& rect);
	// Comprueba si la rana está sobre un nido (Con esta encapsulacion consigo reducir la cantidad de lineas de codigo del metodo checkCollision)
	Collision nestChecking(const SDL_FRect& rect);

	int getRandomRange(int min, int max);

};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
