#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"

class Game;
class Texture;

class Frog {
private:
	Game* gamePointer;
	Texture* texture;
	Point2D<float> position;
	Point2D<float> spawnPosition;
	Vector2D<float> direction;
	int lives;

public:
	Frog(Game* game, Texture* texture, Point2D<float> position, int lives);

	void handleEvent(const SDL_Event& event);
	void Update();
	void setLogDirection(const Vector2D<float>& newDirection);

	int getLives() const;
	Point2D<float> getPosition() const;
	void setPosition(const Point2D<float>& pos);

	void Die();

	void Render() const;
};