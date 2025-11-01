#include "Log.h"
#include "Game.h"
#include "Texture.h"

Log::Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel) :
	gamePointer(game),
	texture(texture),
	position(pos),
	velocity(vel)
{
}

Point2D<float> Log::getPosition() const
{
	return position;
}

Vector2D<float> Log::getVelocity() const
{
	return velocity;
}

void Log::Update()
{
	Point2D<float> actualPos = position;
	Point2D<float> newPos = actualPos + Vector2D<float>(velocity.getX(), velocity.getY());

	position = newPos;

	if (newPos.getX() < Game::leftFrame) {
		position = Vector2D<float>(Game::WINDOW_WIDTH, newPos.getY());
	}
	else if (newPos.getX() + texture->getFrameWidth() > Game::rightFrame) {
		position = Vector2D<float>(-texture->getFrameWidth(), newPos.getY());
	}
}

Collision Log::checkCollision(const SDL_FRect& otherFRect)
{
	SDL_FRect LogRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};

	if (SDL_HasRectIntersectionFloat(&LogRect, &otherFRect)) {
		return Collision(Collision::Type::PLATFORM, velocity);
	}

	return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

void Log::Render() const
{
	SDL_FRect targetFRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};
	texture->renderFrame(targetFRect, 0, 0);
}