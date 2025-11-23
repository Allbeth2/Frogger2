#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL3/SDL.h> // For SDL_Event

class EventHandler {
public:
    virtual ~EventHandler() {}
    virtual void handleEvent(const SDL_Event& event) = 0;
};

#endif // EVENT_HANDLER_H