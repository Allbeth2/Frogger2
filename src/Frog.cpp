#include "Frog.h"
#include "PlayState.h"
#include "Texture.h"
#include <istream>
#include <fstream>
#include "Game.h"
#include "Collision.h"
#include "FileFormatError.h"

/**
 * @brief Constructor de Frog que lee datos desde un archivo.
 * @param state Puntero al PlayState actual.
 * @param file Stream del archivo para leer los datos.
 * @param lineNumber Número de línea actual en el archivo.
 */
Frog::Frog(PlayState* state, std::fstream& file, int lineNumber)
	: SceneObject(state, file, lineNumber),
	  spawnPosition(0.0f, 0.0f),
	  direction(0, 0),
	  lives(3),
	  orientation(1)
{
	int FrogLives;
	// Se aegura que el archivo esta abierto en txt mode
	if (!file.is_open()) {
		throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
	}
	// Se usa std::istream& para el operator>>
	std::istream& in = file;
	if (!(in >> FrogLives)) {
		throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de rana");
	}
	spawnPosition = position;
	lives = FrogLives;
    texture = state->getGame()->getTexture(Game::FROG);
    width = static_cast<float>(texture->getFrameWidth());
    height = static_cast<float>(texture->getFrameHeight());
}

/**
 * @brief Actualiza el estado de la rana, incluyendo movimiento y colisiones.
 */
void Frog::update()
{
	//movimiento
	position = position + direction;
	direction = Vector2D<float>(0, 0);

	SDL_FRect frogRect = getBoundingBox();
	
	//colisiones
	Collision collision = playState->checkCollision(frogRect);

	if (collision.collisionType == Collision::ENEMY) Die();//colision con ENEMIGO
	else if (collision.collisionType == Collision::PLATFORM) setLogDirection(collision.velocity); //colision con PLATAFORMA
	else if (position.getY() < Game::nestHeight){setPosition(spawnPosition);} //limite con zona de nidos
	else if (position.getY() < Game::waterHeight) { Die();} //limite con rio 

	if (position.getX() + width <= 0 || position.getX() >= playState->getGame()->WINDOW_WIDTH) {Die();} //limite con pantalla
}

/**
 * @brief Establece la dirección de movimiento de la rana (usada por los troncos).
 * @param newDirection La nueva dirección.
 */
void Frog::setLogDirection(const Vector2D<float>& newDirection)
{
	direction = newDirection;
}

/**
 * @brief Obtiene el número de vidas de la rana.
 * @return El número de vidas.
 */
int Frog::getLives() const
{
	return lives;
}

/**
 * @brief Reinicia la posición de la rana a su spawn y decrementa una vida.
 */
void Frog::Die()
{
	setPosition(spawnPosition);
	lives--;
}

/**
 * @brief Maneja los eventos de entrada para la rana.
 * @param event El evento de SDL.
 */
void Frog::handleEvent(const SDL_Event& event) //direciones de movimiento de la rana
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
	    switch (event.key.key) {
	    case SDLK_UP:
		    if (position.getY() > 0)
		    {
			    direction = Vector2D<float>(0, -static_cast<float>(Game::CellSize));
			    orientation = 1;
		    }
		    break;
	    case SDLK_DOWN:
		    if (position.getY() + height < Game::WINDOW_HEIGHT)
		    {
			    direction = Vector2D<float>(0, static_cast<float>(Game::CellSize));
			    orientation = 3;
		    }
		    break;
	    case SDLK_LEFT:
		    if (position.getX() > 0) {
			    direction = Vector2D<float>(-static_cast<float>(Game::CellSize), 0);
			    orientation = 2;
		    }
		    break;
	    case SDLK_RIGHT:
		    if (position.getX() + width < Game::WINDOW_WIDTH) {
			    direction = Vector2D<float>(static_cast<float>(Game::CellSize), 0);
			    orientation = 4;
		    }
		    break;
	    default:
		    break;
	    }
    }
}

/**
 * @brief Dibuja la rana en la pantalla.
 */
void Frog::render() const
{
	double angle = 0.0;
	switch (orientation) // So the sprite faces the rigth direction
	{
	case 1: angle = 0.0; break;
	case 2: angle = -90.0; break;
	case 3: angle = 180.0; break;
	case 4: angle = 90.0; break;  
	}
	texture->renderFrame(getBoundingBox(), 0, 0, angle);
}

/**
 * @brief Comprueba la colisión con otro rectángulo.
 * @param otherRect El rectángulo del otro objeto para comprobar la colisión.
 * @return Un objeto Collision que describe la colisión.
 */
Collision Frog::checkCollision(const SDL_FRect& otherRect)
{
	return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
