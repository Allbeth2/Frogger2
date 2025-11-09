#include "Platform.h"
#include "Collision.h"

Collision Platform::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        return Collision(Collision::Type::PLATFORM, getVelocity());
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
