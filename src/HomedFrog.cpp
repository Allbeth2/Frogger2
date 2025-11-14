#include "HomedFrog.h"
#include "Texture.h"

HomedFrog::HomedFrog(Game* game, Texture* texture, Point2D<float> pos)
    : SceneObject(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight())
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
