#include "Crosser.h"
#include "Game.h"
#include "PlayState.h"

Crosser::Crosser(PlayState* state, Texture* texture, Point2D<float> pos, float w, float h, Vector2D<float> vel)
    : SceneObject(state, texture, pos, w, h), velocity(vel)
{
}

Crosser::Crosser(PlayState* state, std::fstream& file, int lineNumber)
    : SceneObject(state, file, lineNumber)
{
    // Derived classes will read from the file
}

//Update
void Crosser::update()
{
    //fisicas
    position = position + velocity;
    
    //Limites
    if (velocity.getX() > 0 && position.getX() >= Game::rightFrame) // Moving right
    {
        float overshoot = position.getX() - Game::rightFrame;
        position.setX(Game::leftFrame - width + overshoot);
    }
    else if (velocity.getX() < 0 && position.getX() + width <= Game::leftFrame) // Moving left
    {
        float overshoot = (position.getX() + width) - Game::leftFrame;
        position.setX(Game::rightFrame + overshoot);
    }
}

Vector2D<float> Crosser::getVelocity() const
{
    return velocity;
}
