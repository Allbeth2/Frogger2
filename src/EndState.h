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

    void handleEvent(const SDL_Event& event) override;

private:
    /**
     * @brief Crea los botones de la interfaz de usuario para la pantalla de fin de juego.
     * @param won True si el jugador ha ganado, false en caso contrario.
     */
    void createButtons(bool won);

    Label* messageLabel_ = nullptr;
    Button* mainMenuButton_ = nullptr;
    Button* exitButton_ = nullptr;
};

#endif // END_STATE_H
