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
#include "TurtleGroup.h"
#include "Wasp.h"
#include "SceneObject.h"
#include "Collision.h"
#include "GameError.h"
#include "FileNotFoundError.h"
#include "SDLError.h"
#include "FileFormatError.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 2.0";
constexpr const char* const MAP_FILE = "../assets/maps/turtles.txt";

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
	TextureSpec{"turtle.png", 1, 7},
	TextureSpec{"wasp.png", 1, 1},
};

//metodo carga las entidades del juego con ayuda de metodos auxiliares
bool Game::LoadEntitiesFromFile()
{
	std::fstream file(MAP_FILE, std::ios::in);
	if (!file.is_open()) //si no puede abrir el archivo salta un error
	{
		throw FileNotFoundError("turtle.txt not found");
		return false;
	}

	char entidad;
	int lineNumber = 0;

	//Se lee el archivo si existe
	while (file >> entidad)
	{
		lineNumber++;
		processEntity(entidad, file, lineNumber);
	}

	deletingWasp = false;
	return true;
}

//Dependiendo de la Letra que identifique a la entidad, llama a la funcion correspondiente de cargar ese objeto
void Game::processEntity(char entidad, std::fstream& file, int lineNumber)
{
	switch (entidad) { // Cada clase se encarga de leer sus propios datos
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
	default:
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
}

//Ingresa los objetos coches a la lista
void Game::loadVehicle(std::fstream& file, int lineNumber)
{
	sceneObjects.push_back(new Vehicle(this, file, lineNumber));
}

//Ingresa los objetos trocos a la lista
void Game::loadLog(std::fstream& file, int lineNumber)
{
	sceneObjects.push_back(new Log(this, file, lineNumber));
}

//Ingresa los objetos tortugas a la lista
void Game::loadTurtleGroup(std::fstream& file, int lineNumber)
{
	sceneObjects.push_back(new TurtleGroup(this, file, lineNumber));
}

//Ingresa el objeto de rana a la lista
void Game::loadFrog(std::fstream& file, int lineNumber)
{
	frogPointer = new Frog(this, file, lineNumber);
	sceneObjects.push_back(frogPointer);
}

Game::Game()
	: exit(false),
	frogPointer(nullptr),
	randomGenerator(time(nullptr))
{
	if (!SDL_Init(SDL_INIT_VIDEO)) //salta un error si SDL no puede inicializar por alguun problema
		throw SDLError("Error al inicializar SDL: "s + SDL_GetError());
	window = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);
	if (window == nullptr) //lanza un error si la ventana no funciona
		throw SDLError("window: "s + SDL_GetError());

	renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr)//lanza un error si el render no funciona
		throw SDLError("renderer: "s + SDL_GetError());


	//Se colocan las texturas adecuadas en donde corresponde cada una
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
	}

	//Nidos
	constexpr float nido0x = 16;
	constexpr float nido0y = 22;
	constexpr float HorizontalGap = 96;

	for (int i = 0; i < 5; ++i) {
		nests.push_back(SDL_FRect{
			nido0x + (i * HorizontalGap),
			nido0y,
			CellSize,
			CellSize - 1
			});
	}
	
	LoadEntitiesFromFile();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{	
	//se destruyen los objectos de la lista
	for(SceneObject * ele : sceneObjects)
	{
		delete ele;
	}

	//Se vacia la lista
	sceneObjects.clear();

	//se destruyen las texturas
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

int Game::getRandomRange(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(randomGenerator);
}

void Game::render() const
{
	SDL_RenderClear(renderer);
	textures[BACKGROUND]->render();

	//se llama al render de cada objeto de la lista
	for (SceneObject* ele : sceneObjects)
	{
		ele->render();
	}

	SDL_RenderPresent(renderer);
}

void Game::update()
{
	//se llama al update de cada objeto de la lista
	for (SceneObject* ele : sceneObjects)
	{
		ele->update();
	}
	trySpawnWasp();

	if (deletingWasp)
	{

		sceneObjects.erase(waspAnchor);
		deletingWasp = false;
	}
}

void Game::run()
{
	// SDL3 devuelve el tiempo en milisegundos, formato Uint64
	Uint64 frameStart = 0;
	Uint64 frameEnd = 0;
	Uint64 frameDuration = 0;

	while (!exit) {

		frameStart = SDL_GetTicks();

		// Logica del juego
		exit = allNestsOccupied() || (frogPointer->getLives() <= 0);
		handleEvents();
		update();
		render();


		// Se calcula el tiempo que ha tomado procesar el juego
		frameEnd = SDL_GetTicks();
		frameDuration = frameEnd - frameStart;

		// Se aplica un delay si el frame fue muy rapido, en caso contrario no se hace nada
		if (frameDuration < DELAYTIME)
		{
			SDL_Delay(static_cast<Uint32>(DELAYTIME - frameDuration));
		}
	}
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		if (event.type == SDL_EVENT_KEY_DOWN) 
		{
			//tecla que indica que se quiere reiniciar el juego
			if (event.key.key == SDLK_0) messageBox();
			
			//cualquier otra tecla presionada llama al HandleEvent de la rana
			if (frogPointer) frogPointer->handleEvent(event);
		}
	}
}

void Game::cleanUp()
{
	for (SceneObject* ele : sceneObjects)
	{
		delete ele;
	}
	sceneObjects.clear();
	frogPointer = nullptr;

	LoadEntitiesFromFile();
}

//muestra la Messagebox
void Game::messageBox()
{
	//array de botones que se muestran en la messagebox
	const SDL_MessageBoxButtonData buttons[] =
	{
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
	};

	//La informacion de la messageBox
	const SDL_MessageBoxData boxdata =
	{
		SDL_MESSAGEBOX_WARNING,
		window,
		"WARNING",
		"Are you sure you want to restart the game?",
		SDL_arraysize(buttons),
		buttons,
		NULL,
	};
	int buttonid = -1;
	if (SDL_ShowMessageBox(&boxdata, &buttonid) && buttonid == 0) cleanUp(); //Si el jugador acepta reiniciar, se llama a la funcion que lo hace posible
}

Collision Game::checkCollision(const SDL_FRect& frogRect)
{
	//cuando la rana llega al nivel de los nidos, se revisan estos
	if (frogRect.y < Game::nestHeight) 
	{
		return nestChecking(frogRect);
	}

	Collision result(Collision::Type::NONE, Vector2D<float>(0, 0));

    for (SceneObject* obj : sceneObjects)
    {
        if (obj != frogPointer) // Don't collide with self
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

void Game::deleteAfter(std::list<SceneObject*>::iterator waspAnchorToDelete)
{
	waspAnchor = waspAnchorToDelete;
	deletingWasp = true;
}
Collision Game::nestChecking (const SDL_FRect& frogRect)
{
	SDL_FRect* nestFound = nullptr;
	for (SDL_FRect& nest : nests) {
		if (SDL_HasRectIntersectionFloat(&frogRect, &nest)) {
			nestFound = &nest;
			break;
		}
	}
	if (nestFound != nullptr) 
	{
		Point2D<float> nestPos(nestFound->x, nestFound->y);
		for (SceneObject* obj : sceneObjects) {
			if (obj->isHomedFrog()) {
				if (obj->getBoundingBox().x == nestPos.getX() && obj->getBoundingBox().y == nestPos.getY()) {
					return Collision(Collision::ENEMY, Vector2D<float>(0,0));
				}
			}
		}
		
		sceneObjects.push_back(new HomedFrog(this, textures[TextureName::FROG], nestPos));
		return Collision(Collision::NONE, Vector2D<float>(0,0));
	}
	return Collision(Collision::ENEMY, Vector2D<float>(0,0));
}

void Game::trySpawnWasp()
{
	int chance = getRandomRange(0, 200);
	if (chance < Game::waspSpawnChance) {
		int nestIndex = getRandomRange(0, nests.size() - 1);
		SDL_FRect& chosenNest = nests[nestIndex];
		Point2D<float> nestPos(chosenNest.x, chosenNest.y);

		bool occupied = false;
		for (SceneObject* so : sceneObjects) {
			if (so->isHomedFrog()) {
				if (so->getBoundingBox().x == nestPos.getX() && so->getBoundingBox().y == nestPos.getY()) {
					occupied = true;
					break;
				}
			}
		}

		if (!occupied) {
			bool waspExists = false;
			for (SceneObject* so : sceneObjects) {
				if (so->isWasp()) {
					waspExists = true;
					break;
				}
			}
			if(!waspExists)
			{

				Uint32 lifetime = getRandomRange(Game::minWaspLifetime, Game::maxWaspLifetime);
				Wasp* newWasp = new Wasp(this, textures[TextureName::WASP], nestPos, Vector2D<float>(0, 0), lifetime);
				waspAnchor = sceneObjects.insert(sceneObjects.end(), newWasp);
				newWasp->setAnchor(waspAnchor);
				
				deletingWasp = false;
			}
		}
	}
}

bool Game::allNestsOccupied() const
{
	int homedFrogCount = 0;
	for(SceneObject* so : sceneObjects)
	{
		if(so->isHomedFrog())
		{
			homedFrogCount++;
		}
	}
	return homedFrogCount == nests.size();
}
