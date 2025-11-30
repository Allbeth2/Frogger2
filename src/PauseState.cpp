#include "PauseState.h"
#include "Game.h"
#include <filesystem>
#include "Button.h"
#include "PlayState.h"
#include "MainMenuState.h"

PauseState::PauseState(Game* game, PlayState* playState)
    : GameState(game), playState_(playState)
{
    createButtons();
}

namespace {
	// Constantes para la posición de los botones
    const int RESUME_BUTTON_X = 90;
    const int RESUME_BUTTON_Y = 100;
    const int RESTART_BUTTON_X = 90;
    const int RESTART_BUTTON_Y = 190;
    const int MAIN_MENU_BUTTON_X = 140;
    const int MAIN_MENU_BUTTON_Y = 280;
    const int EXIT_BUTTON_X = 40;
    const int EXIT_BUTTON_Y = 380;
}

/**
 * @brief Crea los botones de la interfaz de usuario para el menú de pausa.
 */
void PauseState::createButtons()
{
    // Botón de reanudar (CONTINUAR)
    resumeButton_ = new Button(this, game_->getTexture(Game::CONTINUAR), Point2D<float>(Game::WINDOW_WIDTH / 2.0f - RESUME_BUTTON_X, (float)RESUME_BUTTON_Y));
    resumeButton_->connect([this]() {
        game_->popState();
    });
    addObject(resumeButton_);
    addEventListener(resumeButton_);

    // Botón de reiniciar (REINICIAR)
    restartButton_ = new Button(this, game_->getTexture(Game::REINICIAR), Point2D<float>(Game::WINDOW_WIDTH / 2.0f - RESTART_BUTTON_X, (float)RESTART_BUTTON_Y));
    restartButton_->connect([this]() {
        messageBox();
    });
    addObject(restartButton_);
    addEventListener(restartButton_);

    // Botón de volver al menú principal (VOLVER AL MENÚ)
    mainMenuButton_ = new Button(this, game_->getTexture(Game::VOLVER_AL_MENU), Point2D<float>(Game::WINDOW_WIDTH / 2.0f - MAIN_MENU_BUTTON_X, (float)MAIN_MENU_BUTTON_Y));
    mainMenuButton_->connect([this]() {
        game_->popState(); // Pop PauseState
        game_->popState(); // Pop PlayState
    });
    addObject(mainMenuButton_);
    addEventListener(mainMenuButton_);

    // Botón de salir (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>(Game::WINDOW_WIDTH / 2.0f - EXIT_BUTTON_X, (float)EXIT_BUTTON_Y));
    exitButton_->connect([this]() {
        game_->clearStates();
    });
    addObject(exitButton_);
    addEventListener(exitButton_);
}

/**
 * @brief Muestra un cuadro de mensaje de confirmación para reiniciar el juego.
 */
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
		game_->getWindow(), // Usa la ventana del juego como padre
		"WARNING",
		"Are you sure you want to restart the game?",
		SDL_arraysize(buttons),
		buttons,
		nullptr,
	};
	int buttonid = -1;
	if (SDL_ShowMessageBox(&boxdata, &buttonid) && buttonid == 0) {
        game_->popState();
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
    playState_->render();

	// Se renderiza un fondo negro semitransparente encima de el juego pausado
    SDL_SetRenderDrawColor(game_->getRenderer(), 0, 0, 0, 150); // Negro con 155 de alpha (155/255)
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
