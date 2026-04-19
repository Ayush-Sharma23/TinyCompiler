#ifndef EMIT
#define EMIT
#include <string>
#include <iostream>
#include <fstream>

class Emitter
{public:

	std::string fullPath;
	std::string header;
	std::string code;

	Emitter(std::string);

	void emit(std::string);

	void emitLine(std::string);

	void headerLine(std::string);

	void writeFile();
};
#endif
