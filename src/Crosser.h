#ifndef CROSSER_H
#define CROSSER_H

#include "SceneObject.h"
#include "Vector2D.h"
#include <fstream>

class PlayState;

class PlayState;

class Crosser : public SceneObject
{
protected:
    Vector2D<float> velocity;
    Vector2D<float> backjump;
public:
    Crosser(PlayState* state, Texture* texture, Point2D<float> pos, float w, float h, Vector2D<float> vel);
    Crosser(PlayState* state, std::ifstream& file, int lineNumber);
    virtual ~Crosser() = default;
    virtual void update() override;
    Vector2D<float> getVelocity() const;
};

#endif // CROSSER_H