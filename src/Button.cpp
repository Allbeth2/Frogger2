#include "Button.h"
#include "Game.h"
#include <SDL3/SDL.h>

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
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        SDL_FPoint mouseFPos = { (float)event.motion.x, (float)event.motion.y }; // Use SDL_FPoint with float coordinates
        SDL_FRect buttonRect = { position_.getX(), position_.getY(), (float)texture_->getFrameWidth(), (float)texture_->getFrameHeight() };
        mouseOver_ = SDL_PointInRectFloat(&mouseFPos, &buttonRect); // Use SDL_PointInRectFloat
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
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
