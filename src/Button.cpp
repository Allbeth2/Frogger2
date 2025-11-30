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
        SDL_FPoint mousePoint = { (float)event.motion.x, (float)event.motion.y };
        SDL_FRect buttonRect = { position_.getX(), position_.getY(), (float)texture_->getFrameWidth(), (float)texture_->getFrameHeight() };
        mouseOver_ = SDL_PointInRectFloat(&mousePoint, &buttonRect);
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
            // Se ilumina el boton de amarillo cambiando el color del boton
            texture_->render(destRect, SDL_Color{ 255, 255, 0, 255 }); // Amarillo
        }
        else
        {
            texture_->render(destRect);
        }
    }
}
