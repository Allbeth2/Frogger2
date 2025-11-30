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

/**
 * @brief Crea los botones de la interfaz de usuario para la pantalla de fin de juego.
 * @param won True si el jugador ha ganado, false en caso contrario.
 */
void EndState::createButtons(bool won)
{
    // Posiciones de los botones
    const int buttonWidth = 140;
    const int EXIT_BUTTON_X = 60;

    // Etiqueta de mensaje
    Game::TextureName messageTexture = won ? Game::HAS_GANADO : Game::GAME_OVER;
    messageLabel_ = new Label(this, game_->getTexture(messageTexture), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - 100, 150));
    if (won) {
        messageLabel_->setColor({ 0, 255, 0, 255 }); // Verde
    }
    else {
        messageLabel_->setColor({ 255, 0, 0, 255 }); // Rojo
    }
    addObject(messageLabel_);

    // Botón de volver al menú principal (VOLVER AL MENÚ)
    mainMenuButton_ = new Button(this, game_->getTexture(Game::VOLVER_AL_MENU), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth, 250));
    mainMenuButton_->connect([this]() {
        game_->popState(); // Pop EndState
        game_->popState(); // Pop PlayState
    });
    addObject(mainMenuButton_);
    addEventListener(mainMenuButton_);

    // Botón de salir (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - EXIT_BUTTON_X, 350));
    exitButton_->connect([this]() {
        game_->clearStates();
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
    // El fondo se renderiza en negro por defecto porque no se renderiza ninguna textura de fondo
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
