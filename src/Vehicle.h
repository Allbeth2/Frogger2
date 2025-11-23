#ifndef VEHICLE_H
#define VEHICLE_H

#include <istream>
#include "Crosser.h"

class PlayState;

class Vehicle : public Crosser
{
public:
    Vehicle(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel);
    Vehicle(PlayState* state, std::fstream& file, int lineNumber);
    
    ~Vehicle() = default;
    
    // Hereda update() de Crosser
    
    // checkCollision debe hacer override para hacer un return ENEMY collision type
     Collision checkCollision(const SDL_FRect& otherRect) override;
    
    // Hereda render() de SceneObject
    
};

#endif // VEHICLE_H
