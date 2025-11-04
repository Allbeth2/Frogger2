#ifndef WASP_H
#define WASP_H

#include "SceneObject.h"
#include <SDL3/SDL.h>
#include <list>

class Game;


class Wasp : public SceneObject
{
private:
    Vector2D<float> velocity;       
    Uint32 deathTime;               
    std::list<SceneObject*>::iterator anchor;
    
public:
    Wasp(Game* game, Texture* texture, Point2D<float> pos, 
         Vector2D<float> vel, Uint32 lifetime);
    
    virtual ~Wasp() = default;
    
    
    virtual void update() override;
    
    
    virtual void render() const override;
    
    
    virtual Collision checkCollision(const SDL_FRect& otherRect) override;
    
    
    bool isAlive() const;
    
   
    void setAnchor(std::list<SceneObject*>::iterator it);
};

#endif // WASP_H