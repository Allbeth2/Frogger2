#include "Crosser.h"
#include "Game.h" 
#include "PlayState.h"
#include "FileFormatError.h"
Crosser::Crosser(PlayState* state, std::fstream& file, int lineNumber)
    : SceneObject(state, file, lineNumber)
{
    float Xvel;
    if (!(file >> Xvel)) {
        throw FileFormatError("Crosser", lineNumber, "Error al leer la velocidad del Crosser");
    }
    velocity = Vector2D<float>(Xvel / Game::FRAME_RATE, 0.0f);
}

/**
 * @brief Actualiza la posición del objeto y maneja los límites de la pantalla.
 */
void Crosser::update()
{
    position = position + velocity;
    
    //Limites
    if (velocity.getX() > 0 && position.getX() >= Game::rightFrame) // Los que se mueven hacia la derecha
    {
        float overshoot = position.getX() - Game::rightFrame;
        position.setX(Game::leftFrame - width + overshoot);
    }
    else if (velocity.getX() < 0 && position.getX() + width <= Game::leftFrame) // Los que se mueven hacia la izquierda
    {
        float overshoot = (position.getX() + width) - Game::leftFrame;
        position.setX(Game::rightFrame + overshoot);
    }
}

/**
 * @brief Obtiene la velocidad del objeto.
 * @return La velocidad del objeto.
 */
Vector2D<float> Crosser::getVelocity() const
{
    return velocity;
}
