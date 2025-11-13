#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H

#include <istream>

#include "Platform.h"

class TurtleGroup : public Platform
{

    int turtleCount;
    bool Sinking;
    

public:
    TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool Sinking);
    
    virtual ~TurtleGroup() = default;
    
    // Hereda update() de Crosser
    // Hereda checkCollision() de Platform
    void render() const override;


};

#endif // TURTLEGROUP_H
