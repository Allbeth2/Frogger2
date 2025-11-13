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
		int orientation;                      
		
	public:
		Frog(Game* game, Texture* texture, Point2D<float> position, int lives);
			
		~Frog() = default;
		
		void update() override;
		
		void render() const override;
		
		void handleEvent(const SDL_Event& event);

		Collision checkCollision(const SDL_FRect& otherRect) override;
		
		void setLogDirection(const Vector2D<float>& newDirection);
		
		int getLives() const;
		
		void Die();
};

#endif // FROG_H