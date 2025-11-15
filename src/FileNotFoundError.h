#ifndef FILE_NOT_FOUND_ERROR_H
#define FILE_NOT_FOUND_ERROR_H

#include "GameError.h"

class FileNotFoundError: public GameError
{
public: 
	FileNotFoundError(const std::string& arg);
	~FileNotFoundError() = default;
};

#endif
