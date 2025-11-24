#ifndef HOMEDFROG_H
#define HOMEDFROG_H

#include "SceneObject.h"

class PlayState;

class HomedFrog : public SceneObject
{
public:
    HomedFrog(PlayState* state, Texture* texture, Point2D<float> pos);
    
    virtual ~HomedFrog() = default;
    
    // Al heredar de SceneObject, debe tener un override aunque este no haga nada
    void update() override;
    
    void render() const override;
    
    bool isHomedFrog() const override { return true; }
};

#endif // HOMEDFROG_H
