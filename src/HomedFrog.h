#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"

class Game;
class Texture;

class HomedFrog
{
private:
	Game* gamePointer;
	Texture* texture;
	Point2D<float> position;

public:
	HomedFrog(Game* game, Texture* texture, Point2D<float> pos);

	Point2D<float> getPosition() const;

	void Render() const;
};
