#include "Frog.h"
#include "Texture.h"
#include "Game.h"
#include "Collision.h"

// Constructor de la rana, inicializa la posición, textura y las vidas
// @param game Puntero al juego
// @param texture Puntero a la textura de la rana
// @param position Posición inicial de la rana
Frog::Frog(Game* game, Texture* texture, Point2D<float> position, int lives) :
	gamePointer(game),
	texture(texture),
	position(position),
	spawnPosition(position),
	direction(0, 0),
	lives(lives)
{
}
// Obtiene la posición actual de la rana
Point2D<float> Frog::getPosition() const
{
	return position;
}
// Establece una nueva posición para la rana
// @param pos La nueva posición a establecer
void Frog::setPosition(const Point2D<float>& pos)
{
	position = pos;
}
// Actualiza la posición de la rana según su dirección y comprueba si ha salido de los límites
// en ese caso, la rana muere
void Frog::Update()
{
	position = position + direction;
	direction = Vector2D<float>(0, 0);

	float frogXpos = position.getX();

	SDL_FRect frogRect{
			getPosition().getX(),
			getPosition().getY(),
			Game::CellSize-4,
			Game::CellSize-4
		};
		
		Collision collision = gamePointer->checkCollision(frogRect);

		if (collision.collisionType == Collision::ENEMY) Die();
		else if (collision.collisionType == Collision::PLATFORM) setLogDirection(collision.velocity);
		else if (position.getY() < Game::nestHeight) // SI la rana ha sobrevivido en la altura de los nidos, es por que ha caido sobre un nido valido
		{
			setPosition(spawnPosition);
			std::cout << "La rana ha llegado a un nido!" << std::endl;
			return;
		}
		else if (position.getY() < Game::waterHeight) // Si la rana está en el río pero no sobre un tronco, muere
		{
			Die();
			return;
		}
		std::cout << "Rana en posición X: " << frogXpos << ", Y: " << position.getY() << std::endl;

	// Si la rana se sale por los bordes, muere
	if (frogXpos + texture->getFrameWidth() <= 0 || frogXpos >= gamePointer->WINDOW_WIDTH) {
		Die();
	}
}

// Establece la dirección de la rana cuando está sobre un tronco
// @param newDirection La nueva dirección basada en la velocidad del tronco
void Frog::setLogDirection(const Vector2D<float>& newDirection)
{
	direction = Vector2D<float>(newDirection.getX(), newDirection.getY());
}

int Frog::getLives() const
{
	return lives;
}

void Frog::Die()
{
	position = spawnPosition;
	lives--;
}

// Maneja los eventos de teclado para mover la rana, solo cuando es evento de tipo keydown
// @param event El evento SDL a analizar
void Frog::handleEvent(const SDL_Event& event)
{

	switch (event.key.key) {
	case SDLK_UP:
	 if (position.getY() > 0) direction = Vector2D<float>(0, -Game::CellSize);
		break;
	case SDLK_DOWN:
		if (position.getY() + texture->getFrameHeight() < Game::WINDOW_HEIGHT)
		{
			direction = Vector2D<float>(0, Game::CellSize);
		}
		break;
	case SDLK_LEFT:
		if (position.getX() > 0) {
			direction = Vector2D<float>(-Game::CellSize, 0);
		}
		break;
	case SDLK_RIGHT:
		if (position.getX() + texture->getFrameWidth() < Game::WINDOW_WIDTH) {
			direction = Vector2D<float>(Game::CellSize, 0);
		}
		break;
	default:
		break;
	}
}

// Renderiza la rana
void Frog::Render() const
{
	SDL_FRect targetFRect{
		position.getX(),
		position.getY(),
		texture->getFrameWidth(),
		texture->getFrameHeight()
	};
	texture->renderFrame(targetFRect, 0, 0);
}
