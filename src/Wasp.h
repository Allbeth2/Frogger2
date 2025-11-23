#ifndef WASP_H
#define WASP_H

#include "SceneObject.h"
#include <SDL3/SDL.h>
#include <list>

class PlayState;


class Wasp : public SceneObject
{
private:
    Vector2D<float> velocity;       
    Uint32 deathTime;               
    std::list<GameObject*>::iterator itGO_;
    std::list<SceneObject*>::iterator itSCO_;
    
public:
    Wasp(PlayState* state, Texture* texture, Point2D<float> pos, 
         Vector2D<float> vel, Uint32 lifetime);
    
    virtual ~Wasp() = default;
    
    
    virtual void update() override;
    
    
    virtual void render() const override;
    
    
    virtual Collision checkCollision(const SDL_FRect& otherRect) override;
    
    bool isWasp() const override { return true; }
    
    bool isAlive() const;
    
   
    void setIterators(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO);
};

#endif // WASP_H
