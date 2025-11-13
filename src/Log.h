#ifndef LOG_H
#define LOG_H

#include <istream>
#include "Platform.h"

class Log : public Platform
{
public:
    Log(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel);
    
    virtual ~Log() = default;
    
    // Hereda update() de Crosser
    // Hereda checkCollision() de Platform
    // Hereda render() de SceneObject

};

#endif // LOG_H
