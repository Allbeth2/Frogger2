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

bool Game::LoadEntitiesFromFile() 
{
	std::fstream file(MAP_FILE, std::ios::in);
	if (!file.is_open())
	{
		throw FileNotFoundError("turtle.txt not found");
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

void Game::processEntity(char entidad, std::fstream& file, int lineNumber)
{
	float Xpos, Ypos, Xvel;
	int TextureType;
	int turtleCount;
	bool sink;
	int FrogLives;
	
	switch (entidad) {
	case 'V':
		if (!(file >> Xpos >> Ypos >> Xvel >> TextureType)) {
			throw FileFormatError(std::string(MAP_FILE), lineNumber, "Error al leer datos de vehículo");
		}
		loadVehicle(file, lineNumber, Xpos, Ypos, Xvel, TextureType);
		break;
	case 'L':
		if (!(file >> Xpos >> Ypos >> Xvel >> TextureType)) {
			throw FileFormatError(std::string(MAP_FILE), lineNumber, "Error al leer datos de tronco");
		}
		loadLog(file, lineNumber, Xpos, Ypos, Xvel, TextureType);
		break;
	case 'T':
		if (!(file >> Xpos >> Ypos >> Xvel >> turtleCount >> sink)) {
			throw FileFormatError(std::string(MAP_FILE), lineNumber, "Error al leer datos de grupo de tortugas");
		}
		loadTurtleGroup(file, lineNumber, Xpos, Ypos, Xvel, turtleCount, sink);
		break;
	case 'F':
		if (!(file >> Xpos >> Ypos >> FrogLives)) {
			throw FileFormatError(std::string(MAP_FILE), lineNumber, "Error al leer datos de rana");
		}
		loadFrog(file, lineNumber, Xpos, Ypos, FrogLives);
		break;
	default:
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
}

void Game::loadVehicle(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int TextureType)
{
	sceneObjects.push_back(new Vehicle(this, textures[TextureType + 1], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0)));
}

void Game::loadLog(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int TextureType)
{
	sceneObjects.push_back(new Log(this, textures[TextureType + 7], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0)));
}

void Game::loadTurtleGroup(std::fstream& file, int lineNumber, float Xpos, float Ypos, float Xvel, int turtleCount, bool sink)
{
	sceneObjects.push_back(new TurtleGroup(this, textures[TURTLE], Point2D<float>(Xpos, Ypos), Vector2D<float>(Xvel / FRAME_RATE, 0), turtleCount, sink, 0));
}

void Game::loadFrog(std::fstream& file, int lineNumber, float Xpos, float Ypos, int FrogLives)
{
	frogPointer = new Frog(this, textures[FROG], Point2D<float>(Xpos, Ypos), FrogLives);
	sceneObjects.push_back(frogPointer);
}

Game::Game()
	: exit(false),
	frogPointer(nullptr),
	randomGenerator(time(nullptr))
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw SDLError("Error al inicializar SDL: "s + SDL_GetError());
	window = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);
	if (window == nullptr)
		throw SDLError("window: "s + SDL_GetError());

	renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr)
		throw SDLError("renderer: "s + SDL_GetError());

	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
	}

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
	for(SceneObject * ele : sceneObjects)
	{
		delete ele;
	}
	sceneObjects.clear();

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

	for (SceneObject* ele : sceneObjects)
	{
		ele->render();
	}

	SDL_RenderPresent(renderer);
}

void Game::update()
{
	for (SceneObject* ele : sceneObjects)
	{
		ele->update();
	}
	trySpawnWasp();
}

void Game::run()
{
	while (!exit) {
		exit = allNestsOccupied() || (frogPointer->getLives() <= 0);
		handleEvents();
		update();
		render();
		SDL_Delay(DELAYTIME);
	}
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		if (event.type == SDL_EVENT_KEY_DOWN) 
		{
			if (event.key.key == SDLK_0)
			{
				//if ((event.key.mod & SDL_KMOD_SHIFT) && (event.key.mod & SDL_KMOD_CTRL))
				

				for (SceneObject* ele : sceneObjects)
				{
					delete ele;
				}
				sceneObjects.clear();
				frogPointer = nullptr;

				LoadEntitiesFromFile();
				
			}

			frogPointer->handleEvent(event);
			
		}
	}
}

Collision Game::checkCollision(const SDL_FRect& frogRect)
{
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
                return currentCollision; // Enemy collision is highest priority
            }
            if (currentCollision.collisionType == Collision::Type::PLATFORM) {
                result = currentCollision;
            }
        }
    }

	return result;
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
				sceneObjects.push_back(new Wasp(this, textures[TextureName::WASP], nestPos, Vector2D<float>(0, 0), lifetime));
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
