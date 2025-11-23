#include "Wasp.h"
#include "Game.h" // Include Game.h
#include "PlayState.h"
#include "Texture.h"
#include "Collision.h"

Wasp::Wasp(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime)
    : SceneObject(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight()),
      velocity(vel),
      deathTime(SDL_GetTicks() + lifetime)
{
}

void Wasp::update()
{
    position = position + velocity * Game::DELTATIME;

    if (!isAlive())
    {
        playState->deleteAfter(itGO_, itSCO_);
    }
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

void Wasp::setIterators(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO)
{
    itGO_ = itGO;
    itSCO_ = itSCO;
}
