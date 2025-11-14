#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <istream>
#include <SDL3/SDL.h>
#include "GameError.h"

class FileFormatError : public GameError
{
public:

	FileFormatError(const std::string& arg);
	~FileFormatError() = default;
};

#endif
