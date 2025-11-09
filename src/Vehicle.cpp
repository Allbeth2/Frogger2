#include "Vehicle.h"
#include "Texture.h"
#include "Collision.h"

Vehicle::Vehicle(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel)
    : Crosser(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight(), vel)
{
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