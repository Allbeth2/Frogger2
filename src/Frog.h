#ifndef FROG_H
#define FROG_H

#include "SceneObject.h"
#include <SDL3/SDL.h>


class Frog : public SceneObject
{
	private:
		Point2D<float> spawnPosition;  
		Vector2D<float> direction;      
		int lives;                      
		
	public:
		// Este constructor llamará al constructor de SceneObject
		Frog(Game* game, Texture* texture, Point2D<float> position, int lives);
			
		~Frog() = default;
		
		void update();
		
		void render() const override;
		
		void handleEvent(const SDL_Event& event);
		
		void setLogDirection(const Vector2D<float>& newDirection);
		
		int getLives() const;
		Point2D<float> getPosition() const;
		
		void setPosition(const Point2D<float>& pos);
		
		void Die();
};

#endif // FROG_H
