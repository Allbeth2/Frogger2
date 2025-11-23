#include "SceneObject.h"
#include "Texture.h"
#include "PlayState.h" // Include PlayState.h for the definition of PlayState

SceneObject::SceneObject(PlayState* state, Texture* texture, Point2D<float> pos, float w, float h)
    : GameObject(state), playState(state), position(pos), width(w), height(h), texture(texture)
{
}

SceneObject::SceneObject(PlayState* state, std::fstream& file, int lineNumber)
    : GameObject(state), playState(state)
{
    // Derived classes will read from the file
}

SDL_FRect SceneObject::getBoundingBox() const
{
    return { position.getX(), position.getY(), width, height };
}

void SceneObject::render() const
{
    if (texture != nullptr)
    {
        SDL_FRect destRect = getBoundingBox();
        texture->render(destRect);
    }
}

void SceneObject::update()
{
    //SceneObject no usa Update pero sus hijos si
}

Collision SceneObject::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        
        return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

void SceneObject::setPosition(const Point2D<float>& pos)
{
    position = pos;
}
