#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameState;

/**
 * @brief Clase base para todos los objetos del juego.
 */
class GameObject
{
protected:
    GameState *gameState;
    bool visible_ = true;
    GameObject(GameState *state);

public:
    virtual ~GameObject() = default;

    /**
     * @brief Dibuja el objeto en la pantalla.
     */
    virtual void render() const = 0;

    /**
     * @brief Actualiza el estado del objeto.
     */
    virtual void update() = 0;

    /**
     * @brief Establece la visibilidad del objeto.
     * @param visible True si el objeto debe ser visible, false en caso contrario.
     */
    void setVisible(bool visible) { visible_ = visible; }

    /**
     * @brief Comprueba si el objeto es visible.
     * @return True si el objeto es visible, false en caso contrario.
     */
    bool isVisible() const { return visible_; }
};

#endif // GAMEOBJECT_H
