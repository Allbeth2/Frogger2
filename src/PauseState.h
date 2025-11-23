#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "gameState.h"

class Button;
class PlayState;

class PauseState : public GameState
{
public:
    PauseState(Game* game, PlayState* playState);
    virtual ~PauseState() = default;

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

private:
    void createButtons();
    void messageBox();

    PlayState* playState_ = nullptr;
    Button* resumeButton_ = nullptr;
    Button* restartButton_ = nullptr;
    Button* mainMenuButton_ = nullptr;
    Button* exitButton_ = nullptr;
};

#endif // PAUSE_STATE_H