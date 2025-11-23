#ifndef LABEL_H
#define LABEL_H

#include "GameObject.h"
#include "Texture.h"
#include "Vector2D.h"

class Label : public GameObject
{
protected:
    Point2D<float> position_;
    Texture* texture_;

public:
    Label(GameState* state, Texture* texture, Point2D<float> pos);
    virtual ~Label() = default;

    void render() const override;
    void update() override;
    void setTexture(Texture* texture);
};

#endif // LABEL_H