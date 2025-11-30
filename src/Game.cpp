#include "Game.h"
#include <string>
#include <iostream>
#include <array>
#include <SDL3_image/SDL_image.h>
#include "Texture.h"
#include "GameError.h"
#include "SDLError.h"

using namespace std;

constexpr const char* const WINDOW_TITLE = "Frogger 3.0";

Game::Game()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw SDLError("Error al inicializar SDL: " + string(SDL_GetError()));
	window = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);
	if (window == nullptr)
		throw SDLError("window: " + string(SDL_GetError()));

	renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr)
		throw SDLError("renderer: " + string(SDL_GetError()));

	constexpr const char* const imgBase = "../assets/images/";

	// Struct para ayudar a inicializar las texturas
	struct TextureInfo
	{
		const char* name;
		int nrows = 1;
		int ncols = 1;
	};

	const array<TextureInfo, Game::NUM_TEXTURES> textureList{
		TextureInfo{"background.png", 1, 1},
		TextureInfo{"frog.png", 1, 2},
		TextureInfo{"car1.png", 1, 1},
		TextureInfo{"car2.png", 1, 1},
		TextureInfo{"car3.png", 1, 1},
		TextureInfo{"car4.png", 1, 1},
		TextureInfo{"car5.png", 1, 1},
		TextureInfo{"log1.png", 1, 1},
		TextureInfo{"log2.png", 1, 1},
		TextureInfo{"turtle.png", 1, 7},
		TextureInfo{"wasp.png", 1, 1},
        // Texturas del menu principal
        TextureInfo{"menuBackground.png", 1, 1},
        TextureInfo{"texts/CONTINUAR.png", 1, 1},
        TextureInfo{"texts/ELIGE UN MAPA.png", 1, 1},
        TextureInfo{"texts/GAME OVER.png", 1, 1},
        TextureInfo{"texts/HAS GANADO.png", 1, 1},
        TextureInfo{"texts/left.png", 1, 1},
        TextureInfo{"texts/Original.png", 1, 1},
        TextureInfo{"texts/Practica1.png", 1, 1},
        TextureInfo{"texts/Avispado.png", 1, 1},
        TextureInfo{"texts/REINICIAR.png", 1, 1},
        TextureInfo{"texts/right.png", 1, 1},
        TextureInfo{"texts/SALIR.png", 1, 1},
        TextureInfo{"texts/Trivial.png", 1, 1},
        TextureInfo{"texts/Veloz.png", 1, 1},
        TextureInfo{"texts/VOLVER AL MENÚ.png", 1, 1},
	};

	for (size_t i = 0; i < textures.size(); i++) {
		const TextureInfo& spec = textureList[i];
		string path = string(imgBase) + spec.name;
		textures[i] = new Texture(renderer, path.c_str(), spec.nrows, spec.ncols);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
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

void Game::render() const
{
	SDL_RenderClear(renderer);
	GameStateMachine::render(); // Llama a la implementación de la clase base
	SDL_RenderPresent(renderer);
}

void Game::update()
{
	GameStateMachine::update(); // Llama a la implementación de la clase base
}

void Game::handleEvent(const SDL_Event& event)
{
	GameStateMachine::handleEvent(event); // Llama a la implementación de la clase base
}

void Game::handleInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) { // Pasa todos los eventos al estado actual
		handleEvent(event);
	}
}

void Game::run()
{
	// Bucle principal del juego
	Uint64 frameStart = 0;
	Uint64 frameEnd = 0;
	Uint64 frameDuration = 0;

	// Bucle mientras haya estados en la pila
	while (!empty()) {

		frameStart = SDL_GetTicks();

		// Logica del juego
		handleInput();
		update();
		render();

		// Control del framerate
		frameEnd = SDL_GetTicks();
		frameDuration = frameEnd - frameStart;

		// Se aplica un delay si el frame fue muy rapido, en caso contrario no se hace nada
		if (frameDuration < DELAYTIME)
		{
			SDL_Delay(static_cast<Uint32>(DELAYTIME - frameDuration));
		}
	}
}
