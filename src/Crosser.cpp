#include "Crosser.h"
#include "Game.h"

Crosser::Crosser(Game* game, Texture* texture, Point2D<float> pos, float w, float h, Vector2D<float> vel)
    : SceneObject(game, texture, pos, w, h), velocity(vel)
{
}

void Crosser::update()
{
    position = position + velocity;

    // Screen wrapping logic
    if (position.getX() > Game::rightFrame)
    {
        position = Point2D<float>(Game::leftFrame - width, position.getY());
    }
    else if (position.getX() < Game::leftFrame - width)
    {
        position = Point2D<float>(Game::rightFrame, position.getY());
    }
}

Vector2D<float> Crosser::getVelocity() const
{
    return velocity;
}
