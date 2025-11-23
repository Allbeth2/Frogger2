#include "Label.h"

Label::Label(GameState* state, Texture* texture, Point2D<float> pos)
    : GameObject(state), texture_(texture), position_(pos)
{
}

void Label::render() const
{
    if (visible_ && texture_ != nullptr)
    {
        SDL_FRect destRect = { position_.getX(), position_.getY(), (float)texture_->getFrameWidth(), (float)texture_->getFrameHeight() };
        texture_->render(destRect);
    }
}

void Label::update()
{
    // Label does not update
}

void Label::setTexture(Texture* texture)
{
    texture_ = texture;
}