#include "HomedFrog.h"
#include "Texture.h"
#include "PlayState.h" // Include PlayState.h

HomedFrog::HomedFrog(PlayState* state, Texture* texture, Point2D<float> pos)
    : SceneObject(state, texture, pos, static_cast<float>(texture->getFrameWidth()), static_cast<float>(texture->getFrameHeight()))
{
}

void HomedFrog::update()
{
    // HomedFrog does not update.
}

void HomedFrog::render() const
{
    texture->renderFrame(getBoundingBox(), 0, 0);
}
