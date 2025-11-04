#ifndef HOMEDFROG_H
#define HOMEDFROG_H

#include "SceneObject.h"

class HomedFrog : public SceneObject
{
public:
    HomedFrog(Game* game, Texture* texture, Point2D<float> pos);
    
    virtual ~HomedFrog() = default;
    
    // Al heredar de SceneObject, debe
    virtual void update() override;
    
    virtual void render() const override;
    
};

#endif // HOMEDFROG_H