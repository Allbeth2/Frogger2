#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include "gameStateMachine.h"

// Forward declaration for Texture
class Texture;

/**
 * Clase principal del juego.
 * Ahora se comporta como una aplicación SDL y gestiona la máquina de estados.
 */
class Game : private GameStateMachine
{
public:
	// Se actualiza el juego cada tantos milisegundos
	static constexpr float FRAME_RATE = 30.0f;
	static constexpr float DELTATIME = FRAME_RATE / 1000.0f;
	static constexpr float DELAYTIME = 1000.0f /FRAME_RATE;

	static constexpr int WINDOW_WIDTH = 448;
	static constexpr int WINDOW_HEIGHT = 484;

	static constexpr int CellSize = 32;

	static constexpr int FrameLength = 150;
	static constexpr int leftFrame = -FrameLength;
	static constexpr int rightFrame = WINDOW_WIDTH + FrameLength;

	static constexpr int waterHeight = 182;
	static constexpr int nestHeight = 21;

	static constexpr int minWaspLifetime = 3000;
	static constexpr int maxWaspLifetime = 8000;
	static constexpr int waspSpawnChance = 1;

	static constexpr int turtleFrameWidth = 30;

	enum TextureName
	{
		BACKGROUND = 0,
		FROG,
		CAR1, CAR2, CAR3, CAR4, CAR5,
		LOG1, LOG2,
		TURTLE,
		WASP,
        // Menu textures
        MENU_BACKGROUND,
        CONTINUAR,
        ELIGE_UN_MAPA,
        GAME_OVER,
        HAS_GANADO,
        LEFT_ARROW,
        ORIGINAL_MAP_TEXT,
        PRACTICA_1_MAP_TEXT,
        REINICIAR,
        RIGHT_ARROW,
        SALIR,
        TRIVIAL_MAP_TEXT,
        VELOZ_MAP_TEXT,
        VOLVER_AL_MENU,
		NUM_TEXTURES
	};

	Game();
	~Game();

	// Ejecuta el bucle principal del juego
	void run();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;
	// Obtiene el renderer del juego
	SDL_Renderer* getRenderer();

	// Expose GameStateMachine public methods
	using GameStateMachine::empty;
	using GameStateMachine::pushState;
	using GameStateMachine::replaceState;
	using GameStateMachine::popState;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	// Override GameStateMachine's methods
	void update();
	void render() const;
	void handleEvent(const SDL_Event& event);
	void handleInput(); // New method to poll SDL events and pass them to handleEvent(const SDL_Event& event)
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
