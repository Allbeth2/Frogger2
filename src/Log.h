#ifndef LOG_H
#define LOG_H

#include <istream>
#include "Platform.h"

class PlayState;

class Log : public Platform
{
public:
    Log(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel);
	Log(PlayState* state, std::fstream& file, int lineNumber);
    
    virtual ~Log() = default;
    
    // Hereda update() de Crosser
    // Hereda checkCollision() de Platform
    // Hereda render() de SceneObject

};

#endif // LOG_H
