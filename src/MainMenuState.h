#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "gameState.h"
#include <vector>
#include <string>

class Button;

class MainMenuState : public GameState
{
public:
    MainMenuState(Game* game);
    virtual ~MainMenuState() = default;

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

private:
    void loadMaps();
    void createButtons();

    std::vector<std::string> mapFiles_;
    int selectedMap_ = 0;

    // UI elements
    Button* playButton_ = nullptr;
    Button* exitButton_ = nullptr;
    // ... other UI elements like labels for map names ...
};

#endif // MAIN_MENU_STATE_H