#include "Wasp.h"
#include "Game.h"
#include "PlayState.h"
#include "Texture.h"
#include "Collision.h"
#include "FileFormatError.h"
#include <fstream>
#include <istream>

Wasp::Wasp(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime)
    : SceneObject(state, texture, pos, static_cast<float>(texture->getFrameWidth()), static_cast<float>(texture->getFrameHeight())),
      velocity(vel),
      deathTime(SDL_GetTicks() + lifetime)
{
}

Wasp::Wasp(PlayState* state, std::fstream& file, int lineNumber)
    : SceneObject(state, file, lineNumber)
{
    float x, y, vx, vy;
    Uint32 t;

    if (!(file >> x >> y >> vx >> vy >> t))
    {
        throw FileFormatError("Wasp", lineNumber, "Error reading wasp data");
    }

    position = Point2D<float>(x, y);
    velocity = Vector2D<float>(vx / Game::FRAME_RATE, vy / Game::FRAME_RATE);
    deathTime = SDL_GetTicks() + t;
    texture = state->getGame()->getTexture(Game::WASP);
    width = static_cast<float>(texture->getFrameWidth());
    height = static_cast<float>(texture->getFrameHeight());
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