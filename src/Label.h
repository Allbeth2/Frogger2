#ifndef LABEL_H
#define LABEL_H

#include "GameObject.h"
#include "Texture.h"
#include "Vector2D.h"

/**
 * @brief Representa una etiqueta de texto en una posición determinada.
 */
class Label : public GameObject
{
protected:
    Point2D<float> position_;
    Texture* texture_;
    SDL_Color color_ = { 255, 255, 255, 255 }; // Default to white

public:
    Label(GameState* state, Texture* texture, Point2D<float> pos);
    virtual ~Label() = default;

    /**
     * @brief Dibuja la etiqueta en la pantalla.
     */
    void render() const override;

    /**
     * @brief Actualiza el estado de la etiqueta.
     */
    void update() override;

    /**
     * @brief Establece la textura de la etiqueta.
     * @param texture La nueva textura.
     */
    void setTexture(Texture* texture);

    /**
     * @brief Establece el color de la etiqueta.
     * @param color El nuevo color.
     */
    void setColor(SDL_Color color);
};

#endif // LABEL_H
