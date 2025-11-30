#include "HomedFrog.h"
#include "Texture.h"
#include "PlayState.h"


#include "HomedFrog.h"
#include "Texture.h"
#include "PlayState.h"

HomedFrog::HomedFrog(PlayState* state, Texture* texture, Point2D<float> pos)
    : SceneObject(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight())
{
}

/**
 * @brief Actualiza el estado de la homedfrog.
 * HomedFrog no tiene un update especifico.
 */
void HomedFrog::update()
{

}

/**
 * @brief Dibuja la Homedfrog.
 */
void HomedFrog::render() const
{
    texture->renderFrame(getBoundingBox(), 0, 0);
}
