#ifndef FROG_H
#define FROG_H

#include <istream>
#include "SceneObject.h"
#include <SDL3/SDL.h>


#include "EventHandler.h"


class PlayState;

class Frog : public SceneObject, public EventHandler
{
	private:
		Point2D<float> spawnPosition;  
		Vector2D<float> direction;      
		int lives;
		int orientation;                      
		
	public:
		Frog(PlayState* state, std::ifstream& file, int lineNumber);
			
		~Frog() = default;
		
		void update() override;
		
		void render() const override;
		
		void handleEvent(const SDL_Event& event) override;

		Collision checkCollision(const SDL_FRect& otherRect) override;
		
		void setLogDirection(const Vector2D<float>& newDirection);
		
		int getLives() const;
		
		void Die();
};

#endif // FROG_H
