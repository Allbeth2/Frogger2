#include "Log.h"
#include "Texture.h"
#include "Game.h"
#include <istream>
#include <fstream>
#include "FileFormatError.h"

Log::Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Platform(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
}
Log::Log(Game* game, std::fstream& file, int lineNumber)
    : Platform(game,
        game->getTexture(Game::LOG1),
        Point2D<float>(0.0f, 0.0f),
        game->getTexture(Game::LOG1)->getFrameWidth(),
        game->getTexture(Game::LOG1)->getFrameHeight(),
        Vector2D<float>(0.0f, 0.0f))
{
    float Xpos, Ypos, Xvel;
    int TextureType;
    // Ensure file is open and in text mode
    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }
    // Use std::istream& for operator>>
    std::istream& in = file;
    if (!(in >> Xpos >> Ypos >> Xvel >> TextureType)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de tronco");
    }
    int textureIndex = TextureType + 7;
    Texture* tex = game->getTexture(static_cast<Game::TextureName>(textureIndex));
    texture = tex;
    width = tex->getFrameWidth();
    height = tex->getFrameHeight();
    position = Point2D<float>(Xpos, Ypos);
    velocity = Vector2D<float>(Xvel / Game::FRAME_RATE, 0.0f);
}

