#include "Log.h"
#include "Texture.h"
#include "Game.h"
#include "PlayState.h"
#include <istream>
#include <fstream>
#include "FileFormatError.h"

/**
 * @brief Constructor de Log que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
Log::Log(PlayState* state, std::fstream& file, int lineNumber)
    : Platform(state, file, lineNumber)
{
    int TextureType;
    // Si el arhivo no esta abierto en txt, salta error
    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }
    // Usa std::istream& para el operator>>
    std::istream& in = file;
    if (!(in >> TextureType)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de tronco");
    }


    int textureIndex = TextureType + 7;
    Texture* tex = state->getGame()->getTexture(static_cast<Game::TextureName>(textureIndex));
    texture = tex;
    width = tex->getFrameWidth();
    height = tex->getFrameHeight();
}
