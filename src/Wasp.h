#ifndef WASP_H
#define WASP_H

#include "SceneObject.h"
#include <SDL3/SDL.h>
#include <list>

class PlayState;

using GameObjectAnchor = std::list<GameObject*>::iterator;
using SceneObjectAnchor = std::list<SceneObject*>::iterator;


class Wasp : public SceneObject

{

private:

    Vector2D<float> velocity;       

    Uint32 deathTime;               

    GameObjectAnchor itGO_;

    SceneObjectAnchor itSCO_;

    

public:

    

    Wasp(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime);

    

    Wasp(PlayState* state, std::ifstream& file, int lineNumber);

    virtual ~Wasp() = default;
    
    
    virtual void update() override;
    
    
    virtual Collision checkCollision(const SDL_FRect& otherRect) override;
    
    bool isWasp() const override { return true; }
    
    bool isAlive() const;
    
   
    void setIterators(GameObjectAnchor itGO, SceneObjectAnchor itSCO) override;
};

#endif // WASP_H
