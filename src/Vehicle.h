#ifndef VEHICLE_H
#define VEHICLE_H

#include <istream>
#include "Crosser.h"

class Vehicle : public Crosser
{
public:
    Vehicle(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel);
    Vehicle(Game* game, std::fstream& file, int lineNumber);
    
    ~Vehicle() = default;
    
    // Hereda update() de Crosser
    
    // checkCollision debe hacer override para hacer un return ENEMY collision type
     Collision checkCollision(const SDL_FRect& otherRect) override;
    
    // Hereda render() de SceneObject
    
};

#endif // VEHICLE_H
