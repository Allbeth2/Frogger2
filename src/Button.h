#ifndef BUTTON_H
#define BUTTON_H

#include "Label.h"
#include "EventHandler.h"
#include <functional>

/**
 * @brief Un botón de la interfaz de usuario que puede ser pulsado.
 */
class Button : public Label, public EventHandler
{
public:
    using Callback = std::function<void()>;

    Button(GameState* state, Texture* texture, Point2D<float> pos);
    virtual ~Button() = default;

    /**
     * @brief Dibuja el botón en la pantalla.
     */
    void render() const override;

    /**
     * @brief Maneja los eventos de entrada para el botón.
     * @param event El evento de SDL.
     */
    void handleEvent(const SDL_Event& event) override;

    /**
     * @brief Conecta una función de callback al botón.
     * @param cb La función de callback.
     */
    void connect(Callback cb);

private:
    Callback callback_ = nullptr;
    bool mouseOver_ = false;
};

#endif // BUTTON_H