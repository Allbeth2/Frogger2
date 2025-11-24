//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "Game.h"
#include "GameError.h"
#include "SDLError.h"
#include "FileNotFoundError.h"
#include "FileFormatError.h"
#include "MainMenuState.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_messagebox.h>
#include <iostream>
#include <string>
#include <memory>

int main(int argc, char* argv[])
{
	try {
		Game game;
		game.pushState(std::make_shared<MainMenuState>(&game));

		game.run();
	}
	catch (const FileNotFoundError& e) {
		std::cerr << "ERROR: Archivo no encontrado - " << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
		return 1;
	}
	catch (const FileFormatError& e) {
		std::cerr << "ERROR: Formato de archivo incorrecto - " << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
		return 1;
	}
	catch (const SDLError& e) {
		std::cerr << "ERROR: Error de SDL - " << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
		return 1;
	}
	catch (const GameError& e) {
		std::cerr << "ERROR: Error del juego - " << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR INESPERADO: " << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Inesperado", e.what(), nullptr);
		return 1;
	}
	catch (...) {
		std::cerr << "ERROR DESCONOCIDO" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Desconocido", "Se ha producido un error desconocido", nullptr);
		return 1;
	}

	return 0;
}
