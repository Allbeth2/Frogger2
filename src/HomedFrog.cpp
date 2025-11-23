#include "HomedFrog.h"
#include "Texture.h"
#include "PlayState.h" // Include PlayState.h

HomedFrog::HomedFrog(PlayState* state, Texture* texture, Point2D<float> pos)
    : SceneObject(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight())
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
