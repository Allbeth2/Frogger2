#include "Log.h"
#include "Texture.h"

Log::Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Platform(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
}
