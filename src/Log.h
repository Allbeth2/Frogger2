#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"
#include "Collision.h"

class Game;
class Texture;

class Log
{
private:
	Game* gamePointer;
	Texture* texture;
	Point2D<float> position;
	Vector2D<float> velocity;

public:
	Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel);

	Point2D<float> getPosition() const;
	Vector2D<float> getVelocity() const;

	void Render() const;
	void Update();
	Collision checkCollision(const SDL_FRect& otherFRect);
};