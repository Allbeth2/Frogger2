#include "Frog.h"
#include "Texture.h"
#include <istream>
#include <fstream>
#include "Game.h"
#include "Collision.h"
#include "FileFormatError.h"

Frog::Frog(Game* game, Texture* texture, Point2D<float> position, int lives) :
	SceneObject(game, texture, position, texture->getFrameWidth(), texture->getFrameHeight()),
	spawnPosition(position),
	direction(0, 0),
	lives(lives),
	orientation(1)
{
}

Frog::Frog(Game* game, std::fstream& file, int lineNumber)
	: SceneObject(game,
		game->getTexture(Game::FROG),
		Point2D<float>(0.0f, 0.0f),
		game->getTexture(Game::FROG)->getFrameWidth(),
		game->getTexture(Game::FROG)->getFrameHeight()),
	  spawnPosition(0.0f, 0.0f),
	  direction(0, 0),
	  lives(3),
	  orientation(1)
{
	float Xpos, Ypos;
	int FrogLives;
	// Se aegura que el archivo esta abierto en txt mode
	if (!file.is_open()) {
		throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "El archivo no está abierto");
	}
	// Se usa std::istream& para el operator>>
	std::istream& in = file;
	if (!(in >> Xpos >> Ypos >> FrogLives)) {
		throw FileFormatError(std::string("../assets/maps/turtles.txt"), lineNumber, "Error al leer datos de rana");
	}
	position = Point2D<float>(Xpos, Ypos);
	spawnPosition = position;
	lives = FrogLives;
}

void Frog::update()
{
	//movimiento
	position = position + direction;
	direction = Vector2D<float>(0, 0);

	SDL_FRect frogRect = getBoundingBox();
	
	//colisiones
	Collision collision = gamePointer->checkCollision(frogRect);

	if (collision.collisionType == Collision::ENEMY) Die();//colision con ENEMIGO
	else if (collision.collisionType == Collision::PLATFORM) setLogDirection(collision.velocity); //colision con PLATAFORMA
	else if (position.getY() < Game::nestHeight){setPosition(spawnPosition);} //limite con zona de nidos
	else if (position.getY() < Game::waterHeight) { Die();} //limite con rio 

	if (position.getX() + width <= 0 || position.getX() >= gamePointer->WINDOW_WIDTH) {Die();} //limite con pantalla
}

void Frog::setLogDirection(const Vector2D<float>& newDirection)
{
	direction = newDirection;
}

int Frog::getLives() const
{
	return lives;
}

void Frog::Die()
{
	setPosition(spawnPosition);
	lives--;
}

void Frog::handleEvent(const SDL_Event& event) //direciones de movimiento de la rana
{
	switch (event.key.key) {
	case SDLK_UP:
		if (position.getY() > 0)
		{
			direction = Vector2D<float>(0, -Game::CellSize);
			orientation = 1;
		}
		break;
	case SDLK_DOWN:
		if (position.getY() + height < Game::WINDOW_HEIGHT)
		{
			direction = Vector2D<float>(0, Game::CellSize);
			orientation = 3;
		}
		break;
	case SDLK_LEFT:
		if (position.getX() > 0) {
			direction = Vector2D<float>(-Game::CellSize, 0);
			orientation = 2;
		}
		break;
	case SDLK_RIGHT:
		if (position.getX() + width < Game::WINDOW_WIDTH) {
			direction = Vector2D<float>(Game::CellSize, 0);
			orientation = 4;
		}
		break;
	default:
		break;
	}
}

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

Collision Frog::checkCollision(const SDL_FRect& otherRect)
{
	return Collision(Collision::Type::NONE, Vector2D<float>(0, 0));
}
