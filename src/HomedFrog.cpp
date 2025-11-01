#include "HomedFrog.h"
#include "Game.h"
#include "Texture.h"

//Constructir 
HomedFrog::HomedFrog(Game* game, Texture* texture, Point2D<float> pos) :
	gamePointer(game),
	texture(texture),
	position(pos)
{
}

Point2D<float> HomedFrog::getPosition() const
{
	return position;
}

void HomedFrog::Render() const
{
	SDL_FRect targetFRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};
	texture->renderFrame(targetFRect, 0, 0);
}

