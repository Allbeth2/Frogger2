#ifndef CROSSER_H
#define CROSSER_H

#include "SceneObject.h"
#include "Vector2D.h"

class Crosser : public SceneObject
{
protected:
    Vector2D<float> velocity;
    Vector2D<float> backjump;
public:
    Crosser(Game* game, Texture* texture, Point2D<float> pos, float w, float h, Vector2D<float> vel);
    virtual ~Crosser() = default;
    virtual void update() override;
    Vector2D<float> getVelocity() const;
};

#endif // CROSSER_H