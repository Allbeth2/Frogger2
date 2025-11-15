#include "TurtleGroup.h"
#include "Texture.h"
#include "Game.h"
#include <istream>

TurtleGroup::TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool sinking, int currentFrame)
    : Platform(game, texture, pos, Game::turtleFrameWidth * turtleCount, texture->getFrameHeight(), vel),
      turtleCount(turtleCount),
      sinking(sinking), currentFrame(currentFrame), animationTimer(0.0f)
{
    width = texture->getFrameWidth() * turtleCount;
}

void TurtleGroup::update()
{
    Platform::update();

    if (sinking)
    {
        animationTimer += Game::DELTATIME;
        if (animationTimer >= ANIMATION_RATE)
        {
            animationTimer -= ANIMATION_RATE;
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;   //evitas reseteos innecesarios
        }
    }
}

void TurtleGroup::render() const
{
    SDL_FRect destRect = getBoundingBox();
    destRect.w = texture->getFrameWidth();

    const int frames[] = {0, 1, 2, 3, 4, 5, 6};

    for (int i = 0; i < turtleCount; ++i)
    {
        if (!sinking)
        {
            texture->renderFrame(destRect, 0, 0);
        }
        else
        {
            texture->renderFrame(destRect, 0, frames[currentFrame]);
        }
        destRect.x += destRect.w;
    }
}

Collision TurtleGroup :: checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        if (sinking && currentFrame > 3)
        {
            return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
        }
        return Collision(Collision::Type::PLATFORM, getVelocity());
    }
    else 
    {
        return Collision();
    }
}
