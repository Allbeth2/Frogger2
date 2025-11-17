#include "TurtleGroup.h"
#include "Texture.h"
#include "Game.h"
#include <istream>
#include <fstream>
#include "FileFormatError.h"

TurtleGroup::TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool sinking, int currentFrame)
    : Platform(game, texture, pos, Game::turtleFrameWidth * turtleCount, texture->getFrameHeight(), vel),
      turtleCount(turtleCount),
      sinking(sinking), currentFrame(currentFrame), animationTimer(0.0f)
{
    width = texture->getFrameWidth() * turtleCount;
}
TurtleGroup::TurtleGroup(Game* game, std::fstream& file, int lineNumber)
    : Platform(game,
        game->getTexture(Game::TURTLE),
        Point2D<float>(0.0f, 0.0f),
        Game::turtleFrameWidth * 3, // default width for 3 turtles
        game->getTexture(Game::TURTLE)->getFrameHeight(),
        Vector2D<float>(0.0f, 0.0f)),
      turtleCount(3),
      sinking(false), currentFrame(0), animationTimer(0.0f)
{
    float Xpos, Ypos, Xvel;
    int turtleCountInt, sinkInt;
    // Ensure file is open and in text mode
    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }
    // Use std::istream& for operator>>
    std::istream& in = file;
    if (!(in >> Xpos >> Ypos >> Xvel >> turtleCountInt >> sinkInt)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de grupo de tortugas");
    }
    turtleCount = turtleCountInt;
    sinking = (sinkInt != 0);
    width = Game::turtleFrameWidth * turtleCount;
    position = Point2D<float>(Xpos, Ypos);
    velocity = Vector2D<float>(Xvel / Game::FRAME_RATE, 0.0f);
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
