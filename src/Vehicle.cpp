#include <fstream>
#include <istream>
#include "Vehicle.h"
#include "Texture.h"
#include "Collision.h"
#include "Game.h"
#include "PlayState.h"
#include "FileFormatError.h"

/**
 * @brief Constructor de Vehicle que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
Vehicle::Vehicle(PlayState* state, std::fstream& file, int lineNumber) 
    : Crosser(state, file, lineNumber)
{
    int TextureType;


    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }

    std::istream& in = file;
    if (!(in >> TextureType)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de vehículo");
    }

    int textureIndex = TextureType + 1;
    Texture* tex = state->getGame()->getTexture((Game::TextureName)textureIndex);

    texture = tex;
    width = tex->getFrameWidth();
    height = tex->getFrameHeight();
}


/**
 * @brief Comprueba la colisión con otro rectángulo.
 * @param otherRect El rectángulo del otro objeto para comprobar la colisión.
 * @return Un objeto Collision que describe la colisión.
 */
Collision Vehicle::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
