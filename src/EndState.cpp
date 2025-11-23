#include "EndState.h"
#include "Game.h"
#include "Button.h"
#include "Label.h"
#include "MainMenuState.h"

EndState::EndState(Game* game, bool won)
    : GameState(game)
{
    createButtons(won);
}

void EndState::createButtons(bool won)
{
    // Positions are placeholders
    const int buttonWidth = 150;
    const int buttonHeight = 50;

    // Message Label
    Game::TextureName messageTexture = won ? Game::HAS_GANADO : Game::GAME_OVER;
    messageLabel_ = new Label(this, game_->getTexture(messageTexture), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - 100, 150));
    addObject(messageLabel_);

    // Main Menu Button (VOLVER AL MENÚ)
    mainMenuButton_ = new Button(this, game_->getTexture(Game::VOLVER_AL_MENU), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 250));
    mainMenuButton_->connect([this]() {
        game_->popState(); // Pop EndState
        game_->popState(); // Pop PlayState
    });
    addObject(mainMenuButton_);
    addEventListener(mainMenuButton_);

    // Exit Button (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 350));
    exitButton_->connect([this]() {
        while (!game_->empty()) {
            game_->popState();
        }
    });
    addObject(exitButton_);
    addEventListener(exitButton_);
}

void EndState::update()
{
    for (auto& obj : gameObjects_)
    {
        obj->update();
    }
}

void EndState::render() const
{
    game_->getTexture(Game::MENU_BACKGROUND)->render(); // Render a background
    for (const auto& obj : gameObjects_)
    {
        obj->render();
    }
}

void EndState::handleEvent(const SDL_Event& event)
{
    for (auto& handler : eventHandlers_)
    {
        handler->handleEvent(event);
    }
}