#include "TurtleGroup.h"
#include "Texture.h"
#include "Game.h"
#include "PlayState.h"
#include <istream>
#include <fstream>
#include "FileFormatError.h"

/**
 * @brief Constructor de TurtleGroup que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
TurtleGroup::TurtleGroup(PlayState* state, std::fstream& file, int lineNumber)
    : Platform(state, file, lineNumber),
      turtleCount(3),
      sinking(false), currentFrame(0), animationTimer(0.0f)
{
    int turtleCountInt, sinkInt;
    //Se asegura que el arhivo esta abierto
    if (!file.is_open()) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
    }
    // Usa std::istream& para el operator>>
    std::istream& in = file;
    if (!(in >> turtleCountInt >> sinkInt)) {
        throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de grupo de tortugas");
    }
    turtleCount = turtleCountInt;
    sinking = (sinkInt != 0);
    width = static_cast<float>(Game::turtleFrameWidth * turtleCount);
    texture = state->getGame()->getTexture(Game::TURTLE);
    height = static_cast<float>(texture->getFrameHeight());
}

/**
 * @brief Actualiza el estado del grupo de tortugas, incluyendo la animación de hundimiento.
 */
void TurtleGroup::update()
{
    Platform::update();

    if (sinking)//se realiza la animacion si las torutgas se submergen
    {
        animationTimer += Game::DELTATIME; //para que vaya conforme al ritmo del juego
        if (animationTimer >= ANIMATION_RATE) //si el tiempo de animacion de un frame ha exedido el tiempo que debe tener cada frame,  
        {
            animationTimer -= ANIMATION_RATE; //se resta el tiempo excedido
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;   //y se cambia de frame
        }
    }
}

/**
 * @brief Dibuja el grupo de tortugas en la pantalla.
 */
void TurtleGroup::render() const
{
    SDL_FRect destRect = getBoundingBox();
    destRect.w = texture->getFrameWidth();

    const int frames[] = {0, 1, 2, 3, 4, 5, 6}; //los frames que se utilizan para la animacion

    for (int i = 0; i < turtleCount; ++i) //por cada tortuga 
    {
        if (!sinking) texture->renderFrame(destRect, 0, 0);
        else texture->renderFrame(destRect, 0, frames[currentFrame]); //se coloca el frame necesario

        destRect.x += destRect.w; //Se crea la linea de tortugas seguidas
    }
}

/**
 * @brief Comprueba la colisión con otro rectángulo.
 * @param otherRect El rectángulo del otro objeto para comprobar la colisión.
 * @return Un objeto Collision que describe la colisión.
 */
Collision TurtleGroup :: checkCollision(const SDL_FRect& otherRect)
{
    SDL_FRect myRect = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&myRect, &otherRect))
    {
        if (sinking && currentFrame > 3) //Si el frame de las trotugas no es uno de lso 3 primeros, se convierte en enemigo.
        {
            return Collision(Collision::Type::ENEMY, Vector2D<float>(0, 0));
        }
        return Collision(Collision::Type::PLATFORM, getVelocity()); //en caso contrario sigue siendo una plataforma
    }
    else 
    {
        return Collision();
    }
}
