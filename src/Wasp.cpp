#include "Wasp.h"
#include "Game.h"
#include "Texture.h"

Wasp::Wasp(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime) :
	gamePointer(game),
	texture(texture),
	position(pos),
	velocity(vel),
	deathTime(SDL_GetTicks() + lifetime) // Tiempo actual + tiempo de vida
{
}

Point2D<float> Wasp::getPosition() const
{
	return position;
}

Vector2D<float> Wasp::getVelocity() const
{
	return velocity;
}

bool Wasp::isAlive() const
{
	return SDL_GetTicks() < deathTime;
}

void Wasp::Update()
{
	Point2D<float> actualPos = position;
	Point2D<float> newPos = actualPos + Vector2D<float>(velocity.getX(), velocity.getY());
	position = newPos;

}

Collision Wasp::checkCollision(const SDL_FRect& otherFRect)
{
	SDL_FRect waspRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};

	if (SDL_HasRectIntersectionFloat(&waspRect, &otherFRect)) {
		return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
	}

	return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

void Wasp::Render() const
{
	SDL_FRect targetFRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};
	texture->renderFrame(targetFRect, 0, 0);
}