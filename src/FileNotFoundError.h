#ifndef FILE_ERROR_H
#define FILE_ERROR_H


#include <istream>
#include <SDL3/SDL.h>
#include "GameError.h"

class FileNotFoundError: public GameError
{
public: 
	FileNotFoundError(const std::string& arg);
	~FileNotFoundError() = default;
};

#endif
