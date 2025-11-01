#include "Vehicle.h"
#include "Game.h"
#include "Texture.h"

Vehicle::Vehicle(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel) :
    gamePointer(game),
    texture(texture),
    position(pos),
    velocity(vel)
{
}

Point2D<float> Vehicle::getPosition() const
{
    return position;
}

Vector2D<float> Vehicle::getVelocity() const
{
    return velocity;
}

void Vehicle::Update()
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

Collision Vehicle::checkCollision(const SDL_FRect& otherFRect)
{
    SDL_FRect vehicleRect{
        position.getX(),
        position.getY(),
        texture->getFrameWidth(),
        texture->getFrameHeight() 
    };

    if (SDL_HasRectIntersectionFloat(&vehicleRect, &otherFRect)) {
        return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
    }

    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

void Vehicle::Render() const
{
    SDL_FRect targetFRect{
        position.getX(),
        position.getY(),
        texture->getFrameWidth(),
        texture->getFrameHeight()
    };
    texture->renderFrame(targetFRect, 0, 0);
}