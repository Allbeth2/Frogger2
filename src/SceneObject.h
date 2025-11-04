#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GameObject.h"
#include "Vector2D.h"
#include "Collision.h"
#include <SDL3/SDL.h>

class Texture;

/**
 * Base class for all scene objects (objects that appear in the game world).
 * Provides position, dimensions, texture, and collision detection.
 */
class SceneObject : public GameObject
{
protected:
    Point2D<float> position;
    float width;
    float height;
    Texture* texture;
    SceneObject(Game* game, Texture* texture, Point2D<float> pos, float w, float h);
    SDL_FRect getBoundingBox() const;

public:
    virtual ~SceneObject() = default;
    void render() const override;
    void update() override;
    virtual Collision checkCollision(const SDL_FRect& otherRect);
    SDL_FRect getBoundingBox() const;
};

#endif // SCENEOBJECT_H
