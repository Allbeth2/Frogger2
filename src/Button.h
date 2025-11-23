#ifndef BUTTON_H
#define BUTTON_H

#include "Label.h"
#include "EventHandler.h"
#include <functional>

class Button : public Label, public EventHandler
{
public:
    using Callback = std::function<void()>;

    Button(GameState* state, Texture* texture, Point2D<float> pos);
    virtual ~Button() = default;

    void render() const override;
    void handleEvent(const SDL_Event& event) override;
    void connect(Callback cb);

private:
    Callback callback_ = nullptr;
    bool mouseOver_ = false;
};

#endif // BUTTON_H