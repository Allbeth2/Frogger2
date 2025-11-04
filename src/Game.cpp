#include "Game.h"
#include <string>
#include <iostream>
#include <ctime>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <fstream>
#include <limits>
#include "Texture.h"
#include "Frog.h"
#include "HomedFrog.h"
#include "Vehicle.h"
#include "Log.h"
#include "Wasp.h"
#include "SceneObject.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 1.0";
constexpr const char* const MAP_FILE = "../assets/maps/default.txt";

// Estructura para especificar las texturas que hay que cargar
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr const char* const imgBase = "../assets/images/";

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"background.png", 1, 1},
	TextureSpec{"frog.png", 1, 2},
	TextureSpec{"car1.png", 1, 1},
	TextureSpec{"car2.png", 1, 1},
	TextureSpec{"car3.png", 1, 1},
	TextureSpec{"car4.png", 1, 1},
	TextureSpec{"car5.png", 1, 1},
	TextureSpec{"log1.png", 1, 1},
	TextureSpec{"log2.png", 1, 1},
	TextureSpec{"wasp.png", 1, 1},
};

// Carga las entidades del juego desde un archivo de texto
// @param MAP_FILE Ruta al archivo de mapa
// @param vehicles Vector donde se almacenarán los vehículos cargados
// @param logs Vector donde se almacenarán los troncos cargados
// @param frogPointer Puntero donde se almacenará la rana del jugador

bool Game::LoadEntitiesFromFile() 
{
	std::fstream file(MAP_FILE, std::ios::in);
	if (file.is_open()) // Se encontró el archivo correctamente
	{
		char entidad; // "V" , "L" , "F" o "#"
		float Xpos, Ypos, Xvel;
		int TextureType;
		int FrogLives;
		while (file >> entidad) // Mientras que el stream pueda leer (aun queda archivo por leer)
		{
			switch (entidad) {
			case 'V':
				file >> Xpos >> Ypos >> Xvel >> TextureType;
				sceneObjects.push_back(new Vehicle(this, textures[TextureType + 1], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0))); 
				break;
			case 'L':
				file >> Xpos >> Ypos >> Xvel >> TextureType;
				sceneObjects.push_back(new Log(this, textures[TextureType + 7], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0)));
				break;
			case 'F':
				file >> Xpos >> Ypos >> FrogLives;
				frogPointer = new Frog(this, textures[FROG], Point2D<float>(Xpos, Ypos), FrogLives);
				sceneObjects.push_back(frogPointer);
				break;
			default: // Cuando detecta un "#"
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la línea
				break;
			}
		}
		return true; // si todo va bien asd asdasdsdasdasdasdasd
	}
	return false; // si no encuentra el archivo o no lo logra abrir
}




//bool Game::LoadEntitiesFromFile(const char* MAP_FILE, std::vector<Vehicle*>& vehicles, std::vector<Log*>& logs, Frog*& frogPointer) 
//{
//	std::fstream file(MAP_FILE, std::ios::in);
//	if (file.is_open()) // Se encontró el archivo correctamente
//	{
//		char entidad; // "V" , "L" , "F" o "#"
//		float Xpos, Ypos, Xvel;
//		int TextureType;
//		int FrogLives;
//		while (file >> entidad) // Mientras que el stream pueda leer (aun queda archivo por leer)
//		{
//			switch (entidad) {
//			case 'V':
//				file >> Xpos >> Ypos >> Xvel >> TextureType;
//				vehicles.push_back(new Vehicle(this, textures[TextureType + 1], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0))); 
//				break;
//			case 'L':
//				file >> Xpos >> Ypos >> Xvel >> TextureType;
//				logs.push_back(new Log(this, textures[TextureType + 7], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0))); 
//				break;
//			case 'F':
//				file >> Xpos >> Ypos >> FrogLives;
//				frogPointer = new Frog(this, textures[FROG], Point2D<float>(Xpos, Ypos), FrogLives);
//				break;
//			default: // Cuando detecta un "#"
//				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la línea
//				break;
//			}
//		}
//		return true; // si todo va bien asd asdasdsdasdasdasdasd
//	}
//	return false; // si no encuentra el archivo o no lo logra abrir
//}

// Constructor de la clase Game, inicializa SDL, carga texturas y entidades
Game::Game()
	: exit(false),
	frogSpawnPos(Point2D<float>(WINDOW_WIDTH / 2, 405)),
	frogPointer(nullptr),
	randomGenerator(time(nullptr))
	//wasp(nullptr)
{
	// Carga SDL y sus bibliotecas auxiliares

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
		throw "Error al inicializar SDL: "s + SDL_GetError();
	window = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);

	if (window == nullptr)
		throw "window: "s + SDL_GetError();

	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
		throw "renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
	}

	// Inicializa los nidos (5 nidos en la parte superior)
	constexpr float nido0x = 16;
	constexpr float nido0y = 22;
	constexpr float HorizontalGap = 96;

	for (int i = 0; i < 5; ++i) {
		nests.push_back(SDL_FRect{
			nido0x + (i * HorizontalGap),
			nido0y,
			CellSize,
			CellSize - 1 // Para evitar colisiones no intencionadas cuando la rana no está aún en los nidos
			});
	}

	
	LoadEntitiesFromFile();


	// Configura que se pueden utilizar capas translúcidas
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

// Destructor de la clase Game, libera la memoria y cierra SDL
Game::~Game()
{

	for (SceneObject* ele : sceneObjects ) 
	{
		delete ele;
	}

	// puede que la avispa ya haya sido eliminada en un update
	// pero si no, se elimina aquí

	//if (wasp != nullptr) delete wasp;
	

	for (Texture* t : textures) {
		delete t;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Renderer* Game::getRenderer()
{
	return renderer;
}

// Devuelve un número entero aleatorio en el rango [min, max] con igual probabilidad
// @param min Valor mínimo (inclusive)
// @param max Valor máximo (inclusive)
int Game::getRandomRange(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(randomGenerator);
}

// Renderiza todos los elementos del juego
void Game::render() const
{
	SDL_RenderClear(renderer);

	// Renderiza el background
	textures[BACKGROUND]->render();

	// Renderiza los troncos

	for (SceneObject* ele : sceneObjects)
	{
		ele->render();
	}

	// Renderiza las avispas
	/*if (wasp != nullptr) wasp->Render();*/

	SDL_RenderPresent(renderer);
}

// Actualiza todos los elementos del juego
void Game::update()
{
	// Actualiza los troncos
	for (SceneObject* ele : sceneObjects)
	{
		ele->update();
	}

	// Actualiza la avispa
	//if (wasp != nullptr) wasp->Update();
	


	/*if (wasp != nullptr && !(wasp->isAlive())) 
	{
		delete wasp;
		wasp = nullptr;
	}*/
	

	// Intenta generar una nueva avispa aleatoriamente
	trySpawnWasp();
}

// Bucle/pipeline principal del juego
void Game::run()
{
	while (!exit) {
		
		exit = allNestsOccupied() || (frogPointer->getLives() <= 0);
		handleEvents();
		update();
		// El metodo de las colisiones lo llama Frog desde su update
		render();
		SDL_Delay(DELAYTIME);
	}
}

// Maneja los eventos de SDL como el cierre de la ventana o las pulsaciones de teclas
// Si se cierra la ventana, se saldrá del bucle principal en Game::run y el juego terminará
// Si se pulsa una tecla, se delegará el manejo del evento a la rana
void Game::handleEvents() {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		if (event.type == SDL_EVENT_KEY_DOWN) {
			frogPointer->handleEvent(event);
		}
	}
}

// Comprueba si la rana colisiona con algún objeto del juego
// @param frogRect El rectángulo que representa la posición y tamaño de la rana
// @return Devuelve el struct Collision para que la rana pueda actualizar su estado acordemente
Collision Game::checkCollision(const SDL_FRect& frogRect)
{
	// Comprobación de zona de nidos (y < 53)
	if (frogRect.y < Game::nestHeight) 
	{
		return nestChecking(frogRect);
	}

	// Comprueba colisiones con los troncos

	for (SceneObject* ele : sceneObjects)
	{
		ele->checkCollision(frogRect);
	}

	//for (Log* l : logs) 
	//{
	//	Collision result = l->checkCollision(frogRect);
	//	if (result.collisionType == Collision::Type::PLATFORM) {
	//		return result;
	//	}
	//}

	//// Comprueba colisiones con los vehículos
	//for (Vehicle* v : vehicles) {
	//	Collision result = v->checkCollision(frogRect);
	//	if (result.collisionType == Collision::Type::ENEMY) {
	//		return result;	
	//	}
	//}

	//if (wasp != nullptr)
	//{
	//	Collision result = wasp->checkCollision(frogRect);
	//	if (result.collisionType == Collision::Type::ENEMY) {
	//		return result;
	//	}
	//}


	return Collision(Collision::NONE, Vector2D<float>(0,0));
	
}
Collision Game::nestChecking (const SDL_FRect& frogRect)
{
	// Verifica si está sobre un nido válido
		SDL_FRect* nestFound = nullptr;
		for (SDL_FRect& nest : nests) {
			if (SDL_HasRectIntersectionFloat(&frogRect, &nest)) {
				nestFound = &nest;
				break;
			}
		}
		if (nestFound != nullptr) 
		{
			// Verifica si el nido ya está ocupado
			bool isOccupied = false;
			Point2D<float> nestPos(nestFound->x, nestFound->y);

			for (HomedFrog* hf : homedFrogs) {
				if (hf->getPosition() == nestPos) { // Nido ocupado, muere
					return Collision(Collision::ENEMY, Vector2D<float>(0,0));
				}
			}

			// Nido libre, crea una HomedFrog y resetea la rana
			homedFrogs.push_back(new HomedFrog(this, textures[TextureName::FROG], nestPos));
			return Collision(Collision::NONE, Vector2D<float>(0,0));
		}
		return Collision(Collision::ENEMY, Vector2D<float>(0,0));
}
// Intenta generar una nueva avispa aleatoriamente
// con baja probabilidad en cada frame (1 entre 200)
void Game::trySpawnWasp()
{
	// Probabilidad baja de generar una avispa en cada frame
	int chance = getRandomRange(0, 100);

	if (chance < Game::waspSpawnChance) { // 1% de probabilidad
		if (wasp != nullptr) return;

		// Elige un nido aleatorio
		int nestIndex = getRandomRange(0, nests.size() - 1);
		SDL_FRect& chosenNest = nests[nestIndex];
		Point2D<float> nestPos(chosenNest.x, chosenNest.y);

		// Verifica si el nido está ocupado por una HomedFrog
		bool occupied = false;
		for (HomedFrog* hf : homedFrogs) {
			if (hf->getPosition() == nestPos) {
				occupied = true;
				break;
			}
		}

		// Si el nido no está ocupado, crea la avispa
		if (!occupied) {
			Uint32 lifetime = getRandomRange(Game::minWaspLifetime, Game::maxWaspLifetime);

			wasp = new Wasp(this, textures[TextureName::WASP], nestPos, Vector2D<float>(0, 0), lifetime);
		}
	}
}
// Comprueba si todos los nidos están ocupados
// @return true si todos los nidos están ocupados, false en caso contrario
bool Game::allNestsOccupied() const
{

	if (homedFrogs.size() != nests.size()) return false;

	return true;
}
