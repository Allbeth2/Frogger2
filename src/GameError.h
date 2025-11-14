#ifndef GAME_ERROR_H
#define GAME_ERROR_H


#include <istream>
#include <SDL3/SDL.h>

class GameError : public  std::logic_error
{
public:
	GameError(const std::string& arg);
	~GameError() = default;

};

#endif
