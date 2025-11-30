#include "SceneObject.h"
#include "Texture.h"
#include "PlayState.h"
#include "FileFormatError.h"
#include <fstream>
SceneObject::SceneObject(PlayState* state, Texture* texture, Point2D<float> pos, float w, float h)
    : GameObject(state), position(pos), width(w), height(h), texture(texture), playState(state)
{
}

/**
 * @brief Constructor de SceneObject que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
SceneObject::SceneObject(PlayState* state, std::fstream& file, int lineNumber)
    : GameObject(state), playState(state)
{
    float Xpos, Ypos;

    if (!(file >> Xpos >> Ypos)) {
        throw FileFormatError("SceneObject", lineNumber, "Error al leer los datos de posición");
    }
    position = Point2D<float>(Xpos, Ypos);
}

/**
 * @brief Obtiene el rectángulo delimitador del objeto.
 * @return Un SDL_FRect que representa el rectángulo delimitador.
 */
SDL_FRect SceneObject::getBoundingBox() const
{
    return { position.getX(), position.getY(), width, height };
}

/**
 * @brief Dibuja el objeto en la pantalla si es visible.
 */
void SceneObject::render() const
{
    if (visible_ && texture != nullptr)
    {
        SDL_FRect destRect = getBoundingBox();
        texture->render(destRect);
    }
}

/**
 * @brief Actualiza el estado del objeto.
 * Los SceneObjects base no tienen un update especifico,
 * pero sus clases derivadas pueden sobrescribirla.
 */
void SceneObject::update()
{
    //SceneObject no usa Update pero sus hijos si
}

/**
 * @brief Comprueba la colisión con otro rectángulo.
 * @param otherRect El rectángulo del otro objeto para comprobar la colisión.
 * @return Un objeto Collision que describe la colisión.
 */
Collision SceneObject::checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        
        return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
    }
    return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}

/**
 * @brief Establece la posición del objeto.
 * @param pos La nueva posición.
 */
void SceneObject::setPosition(const Point2D<float>& pos)
{
    position = pos;
}
