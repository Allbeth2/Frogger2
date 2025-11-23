#include <fstream>
#include <istream>
#include "Vehicle.h"
#include "Texture.h"
#include "Collision.h"
#include "Game.h"
#include "PlayState.h" // Include PlayState.h
#include "FileFormatError.h"

Vehicle::Vehicle(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Crosser(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
}

Vehicle::Vehicle(PlayState* state, std::fstream& file, int lineNumber) 
    : Crosser(state, file, lineNumber)
{
    float Xpos, Ypos, Xvel;
    int TextureType;


    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }

    std::istream& in = file;
    if (!(in >> Xpos >> Ypos >> Xvel >> TextureType)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de vehículo");
    }

    int textureIndex = TextureType + 1;
    Texture* tex = state->getGame()->getTexture(static_cast<Game::TextureName>(textureIndex));

    texture = tex;
    width = tex->getFrameWidth();
    height = tex->getFrameHeight();
    position = Point2D<float>(Xpos, Ypos);
    velocity = Vector2D<float>(Xvel / Game::FRAME_RATE, 0.0f);
}


Collision Vehicle::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
