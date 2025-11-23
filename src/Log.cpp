#include "Log.h"
#include "Texture.h"
#include "Game.h"
#include "PlayState.h" // Include PlayState.h
#include <istream>
#include <fstream>
#include "FileFormatError.h"

Log::Log(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Platform(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
}

//constructor para que log obtenga sus parametros por si mismo
Log::Log(PlayState* state, std::fstream& file, int lineNumber)
    : Platform(state, file, lineNumber)
{
    float Xpos, Ypos, Xvel;
    int TextureType;
    // Si el arhivo no esta abierto en txt, salta error
    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }
    // Usa std::istream& para el operator>>
    std::istream& in = file;
    if (!(in >> Xpos >> Ypos >> Xvel >> TextureType)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de tronco");
    }
    int textureIndex = TextureType + 7;
    Texture* tex = state->getGame()->getTexture(static_cast<Game::TextureName>(textureIndex));
    texture = tex;
    width = tex->getFrameWidth();
    height = tex->getFrameHeight();
    position = Point2D<float>(Xpos, Ypos);
    velocity = Vector2D<float>(Xvel / Game::FRAME_RATE, 0.0f);
}

