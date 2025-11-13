#include "TurtleGroup.h"
#include "Texture.h"
#include "Game.h"
#include <istream>

TurtleGroup::TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool Sinking)
    : Platform(game, texture, pos, Game::turtleFrameWidth * turtleCount, texture->getFrameHeight(), vel),
      turtleCount(turtleCount),
      Sinking(Sinking)
{
    width = texture->getFrameWidth() * turtleCount;
}

void TurtleGroup::render() const
{
    SDL_FRect destRect = getBoundingBox();
    destRect.w = texture->getFrameWidth();

    for (int i = 0; i < turtleCount; ++i)
    {
        texture->renderFrame(destRect, 0, 0);
        destRect.x += destRect.w;
    }
}
