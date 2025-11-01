//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "Game.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	try {
		Game game;
		game.run();
	}
	catch (const std::string& error) {
		std::cerr << "ERROR: " << error << std::endl;
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR INESPERADO: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "ERROR DESCONOCIDO" << std::endl;
		return 1;
	}

	return 0;
}
