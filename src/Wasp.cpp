#include "Wasp.h"
#include "Game.h"
#include "PlayState.h"
#include "Texture.h"
#include "Collision.h"
#include "FileFormatError.h"
#include <fstream>
#include <istream>

namespace {
	// Multiplicador para que las avispas tengan una velocidad adecuada (5 o 4 pixeles por segundo es excesivamente lento)
    constexpr float WASP_SPEED_MULTIPLIER = 30.0f;
}

Wasp::Wasp(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel, Uint32 lifetime)
    : SceneObject(state, texture, pos, texture->getFrameWidth(), texture->getFrameHeight()),
      velocity(vel),
      deathTime(SDL_GetTicks() + lifetime)
{
}

/**
 * @brief Constructor de Wasp que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
Wasp::Wasp(PlayState* state, std::fstream& file, int lineNumber)
    : SceneObject(state, file, lineNumber)
{
    float vx, vy;
    Uint32 t;

    if (!(file >> vx >> vy >> t))
    {
        throw FileFormatError("Wasp", lineNumber, "Error al leer los datos de la avispa");
    }

    velocity = Vector2D<float>(vx * WASP_SPEED_MULTIPLIER / Game::FRAME_RATE, vy * WASP_SPEED_MULTIPLIER / Game::FRAME_RATE);
    deathTime = SDL_GetTicks() + t;
    texture = state->getGame()->getTexture(Game::WASP);
    width = texture->getFrameWidth();
    height = texture->getFrameHeight();
}

/**
 * @brief Actualiza el estado de la avispa.
 */
void Wasp::update()
{
    position = position + velocity;

    if (!isAlive())
    {
        playState->deleteAfter(itGO_, itSCO_);
    }
}

/**
 * @brief Comprueba si la avispa está viva.
 * @return True si la avispa está viva, false en caso contrario.
 */
bool Wasp::isAlive() const
{
    return SDL_GetTicks() < deathTime;
}

/**
 * @brief Comprueba la colisión con otro rectángulo.
 * @param otherRect El rectángulo del otro objeto para comprobar la colisión.
 * @return Un objeto Collision que describe la colisión.
 */
Collision Wasp::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

/**
 * @brief Dibuja la avispa en la pantalla.
 */
void Wasp::render() const
{
    SceneObject::render();
}

/**
 * @brief Establece los iteradores para la avispa en las listas de objetos del juego.
 * @param itGO Iterador en la lista gameObjects_.
 * @param itSCO Iterador en la lista sceneObjectsForCollision_.
 */
void Wasp::setIterators(std::list<GameObject*>::iterator itGO, std::list<SceneObject*>::iterator itSCO)
{
    itGO_ = itGO;
    itSCO_ = itSCO;
}
