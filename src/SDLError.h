#ifndef SDL_ERROR_H
#define SDL_ERROR_H

#include <istream>
#include "GameError.h"


class SDLError : public GameError
{
public:

	SDLError(const std::string& arg);
	~SDLError() = default;
};

#endif
