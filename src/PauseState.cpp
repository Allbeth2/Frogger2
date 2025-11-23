#include "PauseState.h"
#include "Game.h"
#include "Button.h"
#include "PlayState.h"
#include "MainMenuState.h"

PauseState::PauseState(Game* game, PlayState* playState)
    : GameState(game), playState_(playState)
{
    createButtons();
}

void PauseState::createButtons()
{
    // Positions are placeholders
    const int buttonWidth = 150;
    const int buttonHeight = 50;

    // Resume Button (CONTINUAR)
    resumeButton_ = new Button(this, game_->getTexture(Game::CONTINUAR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 150));
    resumeButton_->connect([this]() {
        game_->popState();
    });
    addObject(resumeButton_);
    addEventListener(resumeButton_);

    // Restart Button (REINICIAR)
    restartButton_ = new Button(this, game_->getTexture(Game::REINICIAR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 250));
    restartButton_->connect([this]() {
        messageBox();
    });
    addObject(restartButton_);
    addEventListener(restartButton_);

    // Main Menu Button (VOLVER AL MENÚ)
    mainMenuButton_ = new Button(this, game_->getTexture(Game::VOLVER_AL_MENU), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 350));
    mainMenuButton_->connect([this]() {
        game_->popState(); // Pop PauseState
        game_->popState(); // Pop PlayState
    });
    addObject(mainMenuButton_);
    addEventListener(mainMenuButton_);

    // Exit Button (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 400));
    exitButton_->connect([this]() {
        while (!game_->empty()) {
            game_->popState();
        }
    });
    addObject(exitButton_);
    addEventListener(exitButton_);
}

void PauseState::messageBox()
{
	const SDL_MessageBoxButtonData buttons[] =
	{
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
	};

	const SDL_MessageBoxData boxdata =
	{
		SDL_MESSAGEBOX_WARNING,
		game_->getWindow(), // Use game window as parent
		"WARNING",
		"Are you sure you want to restart the game?",
		SDL_arraysize(buttons),
		buttons,
		nullptr,
	};
	int buttonid = -1;
	if (SDL_ShowMessageBox(&boxdata, &buttonid) && buttonid == 0) {
        game_->popState(); // Pop PauseState
        game_->replaceState(std::make_shared<PlayState>(game_, playState_->getMapFilePath()));
    }
}

void PauseState::update()
{
    for (auto& obj : gameObjects_)
    {
        obj->update();
    }
}

void PauseState::render() const
{
    playState_->render(); // Render the playstate underneath

    // Render a semi-transparent black overlay
    SDL_SetRenderDrawColor(game_->getRenderer(), 0, 0, 0, 150); // Black with alpha 150
    SDL_FRect overlayRect = { 0, 0, (float)Game::WINDOW_WIDTH, (float)Game::WINDOW_HEIGHT };
    SDL_RenderFillRect(game_->getRenderer(), &overlayRect);

    for (const auto& obj : gameObjects_)
    {
        obj->render();
    }
}

void PauseState::handleEvent(const SDL_Event& event)
{
    for (auto& handler : eventHandlers_)
    {
        handler->handleEvent(event);
    }
}
