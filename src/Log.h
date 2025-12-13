#ifndef LOG_H
#define LOG_H

#include <istream>
#include "Platform.h"

class PlayState;

class Log : public Platform
{
public:
	Log(PlayState* state, std::ifstream& file, int lineNumber);
    
    virtual ~Log() = default;
    
    // Hereda update() de Crosser
    // Hereda checkCollision() de Platform
    // Hereda render() de SceneObject

};

#endif // LOG_H
