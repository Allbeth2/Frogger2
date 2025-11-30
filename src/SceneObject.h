#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GameObject.h"
#include "Vector2D.h"
#include "Collision.h"
#include <list>
#include <SDL3/SDL.h>

class Texture;

class PlayState;

/**
 * Classe base para todos los objetos de la escena que tienen posición, tamaño y textura.
 * 
 */
class SceneObject : public GameObject
{
protected:
    Point2D<float> position;
    float width;
    float height;
    Texture* texture;
    PlayState* playState;
    SceneObject(PlayState* state, Texture* texture, Point2D<float> pos, float w, float h);
    SceneObject(PlayState* state, std::fstream& file, int lineNumber);

public:
    virtual ~SceneObject() = default;
    void render() const override;
    void update() override;
    virtual Collision checkCollision(const SDL_FRect& otherRect);
    SDL_FRect getBoundingBox() const;
    void setPosition(const Point2D<float>& pos);
    
    virtual void setIterators(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO) {}

    virtual bool isHomedFrog() const { return false; }
    virtual bool isWasp() const { return false; }
};

#endif // SCENEOBJECT_H
