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

	constexpr const char* const imgBase = "../assets/images/";

	struct TextureSpec
	{
		const char* name;
		int nrows = 1;
		int ncols = 1;
	};

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
        // Texturas del menu principal
        TextureSpec{"menuBackground.png", 1, 1},
        TextureSpec{"texts/CONTINUAR.png", 1, 1},
        TextureSpec{"texts/ELIGE UN MAPA.png", 1, 1},
        TextureSpec{"texts/GAME OVER.png", 1, 1},
        TextureSpec{"texts/HAS GANADO.png", 1, 1},
        TextureSpec{"texts/left.png", 1, 1},
        TextureSpec{"texts/Original.png", 1, 1},
        TextureSpec{"texts/Practica1.png", 1, 1},
        TextureSpec{"texts/Avispado.png", 1, 1}, 
        TextureSpec{"texts/REINICIAR.png", 1, 1},
        TextureSpec{"texts/right.png", 1, 1},
        TextureSpec{"texts/SALIR.png", 1, 1},
        TextureSpec{"texts/Trivial.png", 1, 1},
        TextureSpec{"texts/Veloz.png", 1, 1},
        TextureSpec{"texts/VOLVER AL MENÚ.png", 1, 1},
	};

	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
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
	GameStateMachine::render(); // Delega el render a GameStateMachine
	SDL_RenderPresent(renderer);
}

void Game::update()
{
	GameStateMachine::update(); // Delega el update a GameStateMachine
}

void Game::handleEvent(const SDL_Event& event)
{
	GameStateMachine::handleEvent(event); // Delegate to GameStateMachine's handleEvent
}

void Game::handleInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Pass all events to the current state
		handleEvent(event);
	}
}

void Game::run()
{
	// SDL3 devuelve el tiempo en milisegundos, formato Uint64
	Uint64 frameStart = 0;
	Uint64 frameEnd = 0;
	Uint64 frameDuration = 0;

	// Bucle mientras haya estados en la pila
	while (!GameStateMachine::empty()) {

		frameStart = SDL_GetTicks();

		// La logica del juego
		handleInput();
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
