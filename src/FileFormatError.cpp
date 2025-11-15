#include "FileFormatError.h"
#include <sstream>

FileFormatError::FileFormatError(const std::string& filename, int lineNumber, const std::string& message)
	: GameError(buildMessage(filename, lineNumber, message)), filename(filename), lineNumber(lineNumber)
{
}

std::string FileFormatError::buildMessage(const std::string& filename, int lineNumber, const std::string& message)
{
	std::ostringstream oss;
	oss << "Error en archivo '" << filename << "' línea " << lineNumber << ": " << message;
	return oss.str();
}
