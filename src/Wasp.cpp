#include "Wasp.h"
#include "Game.h"
#include "Texture.h"
#include "Collision.h"

Wasp::Wasp(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime)
    : SceneObject(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight()),
      velocity(vel),
      deathTime(SDL_GetTicks() + lifetime)
{
}

void Wasp::update()
{
    position = position + velocity * Game::DELTATIME;
    // The logic for self-destruction will be handled by the Game class
}

bool Wasp::isAlive() const
{
    return SDL_GetTicks() < deathTime;
}

Collision Wasp::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

void Wasp::render() const
{
    SceneObject::render();
}

//extension practica 2
void Wasp::setAnchor(std::list<SceneObject*>::iterator it)
{
    anchor = it;
}
