#ifndef END_STATE_H
#define END_STATE_H

#include "gameState.h"

class Button;
class Label;

class EndState : public GameState
{
public:
    EndState(Game* game, bool won);
    virtual ~EndState() = default;

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

private:
    void createButtons(bool won);

    Label* messageLabel_ = nullptr;
    Button* mainMenuButton_ = nullptr;
    Button* exitButton_ = nullptr;
};

#endif // END_STATE_H