#include "Log.h"
#include "Texture.h"
#include "Game.h"
#include <istream>

Log::Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Platform(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
}

