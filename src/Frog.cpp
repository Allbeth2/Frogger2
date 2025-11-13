#include "Frog.h"
#include "Texture.h"
#include <istream>
#include "Game.h"
#include "Collision.h"

Frog::Frog(Game* game, Texture* texture, Point2D<float> position, int lives) :
	SceneObject(game, texture, position, texture->getFrameWidth(), texture->getFrameHeight()),
	spawnPosition(position),
	direction(0, 0),
	lives(lives),
	orientation(1)
{
}

void Frog::update()
{
	position = position + direction;
	direction = Vector2D<float>(0, 0);

	SDL_FRect frogRect = getBoundingBox();
		
	Collision collision = gamePointer->checkCollision(frogRect);

	if (collision.collisionType == Collision::ENEMY) Die();
	else if (collision.collisionType == Collision::PLATFORM) setLogDirection(collision.velocity);
	else if (position.getY() < Game::nestHeight)
	{
		setPosition(spawnPosition);
	}
	else if (position.getY() < Game::waterHeight)
	{
		Die();
	}

	if (position.getX() + width <= 0 || position.getX() >= gamePointer->WINDOW_WIDTH) {
		Die();
	}
}

void Frog::setLogDirection(const Vector2D<float>& newDirection)
{
	direction = newDirection;
}

int Frog::getLives() const
{
	return lives;
}

void Frog::Die()
{
	setPosition(spawnPosition);
	lives--;
}

void Frog::handleEvent(const SDL_Event& event)
{
	switch (event.key.key) {
	case SDLK_UP:
		if (position.getY() > 0)
		{
			direction = Vector2D<float>(0, -Game::CellSize);
			orientation = 1;
		}
		break;
	case SDLK_DOWN:
		if (position.getY() + height < Game::WINDOW_HEIGHT)
		{
			direction = Vector2D<float>(0, Game::CellSize);
			orientation = 3;
		}
		break;
	case SDLK_LEFT:
		if (position.getX() > 0) {
			direction = Vector2D<float>(-Game::CellSize, 0);
			orientation = 2;
		}
		break;
	case SDLK_RIGHT:
		if (position.getX() + width < Game::WINDOW_WIDTH) {
			direction = Vector2D<float>(Game::CellSize, 0);
			orientation = 4;
		}
		break;
	default:
		break;
	}
}

void Frog::render() const
{
	double angle = 0.0;
	switch (orientation)
	{
	case 1: angle = 0.0; break;
	case 2: angle = -90.0; break;
	case 3: angle = 180.0; break;
	case 4: angle = 90.0; break;  
	}
	texture->renderFrame(getBoundingBox(), 0, 0, angle);
}

Collision Frog::checkCollision(const SDL_FRect& otherRect)
{
	return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
