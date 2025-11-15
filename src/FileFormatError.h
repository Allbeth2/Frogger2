#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include "GameError.h"
#include <string>

class FileFormatError : public GameError
{
private:
	std::string filename;
	int lineNumber;
	
	static std::string buildMessage(const std::string& filename, int lineNumber, const std::string& message);

public:
	FileFormatError(const std::string& filename, int lineNumber, const std::string& message);
	~FileFormatError() = default;
	
	const std::string& getFilename() const { return filename; }
	int getLineNumber() const { return lineNumber; }
};

#endif
