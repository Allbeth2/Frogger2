#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"
#include "Collision.h"

class Game;
class Texture;

class Wasp
{
private:
	Game* gamePointer;
	Texture* texture;
	Point2D<float> position;
	Vector2D<float> velocity;
	Uint32 deathTime; // Momento en que la avispa debe desaparecer

public:
	Wasp(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime);

	Point2D<float> getPosition() const;
	Vector2D<float> getVelocity() const;

	bool isAlive() const;

	void Render() const;
	void Update();
	Collision checkCollision(const SDL_FRect& otherFRect);
};

