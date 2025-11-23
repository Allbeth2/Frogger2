#include "Button.h"
#include "Game.h"

Button::Button(GameState* state, Texture* texture, Point2D<float> pos)
    : Label(state, texture, pos)
{
}

void Button::connect(Callback cb)
{
    callback_ = cb;
}

void Button::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSEMOTION)
    {
        SDL_Point mousePos = { event.motion.x, event.motion.y };
        SDL_FRect buttonRect = { position_.getX(), position_.getY(), (float)texture_->getFrameWidth(), (float)texture_->getFrameHeight() };
        mouseOver_ = SDL_PointInFRect(&mousePos, &buttonRect);
    }
    else if (event.type == SDL_EVENT_MOUSEBUTTONDOWN)
    {
        if (mouseOver_ && callback_ != nullptr)
        {
            callback_();
        }
    }
}

void Button::render() const
{
    if (texture_ != nullptr)
    {
        SDL_FRect destRect = { position_.getX(), position_.getY(), (float)texture_->getFrameWidth(), (float)texture_->getFrameHeight() };
        if (mouseOver_)
        {
            // Highlight the button by changing its color
            texture_->render(destRect, { 255, 255, 0, 255 }); // Yellow highlight
        }
        else
        {
            texture_->render(destRect);
        }
    }
}