#include "SceneObject.h"
#include "Texture.h"
#include "Game.h"

SceneObject::SceneObject(Game* game, Texture* texture, Point2D<float> pos, float w, float h)
    : GameObject(game), position(pos), width(w), height(h), texture(texture)
{
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