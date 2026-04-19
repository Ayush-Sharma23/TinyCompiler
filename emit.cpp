#include "emit.h"
Emitter::Emitter(std::string fullPath) {
	this->fullPath = fullPath;
}

void Emitter::emit(std::string code) {
	this->code.append(code);
}

void Emitter::emitLine(std::string code) {
	this->code.append(code);
	this->code.push_back('\n');
}

void Emitter::headerLine(std::string code) {
	this->header.append(code);
	this->header.push_back('\n');
}

void Emitter::writeFile() {
	std::ofstream outputFile(this->fullPath);
	if(outputFile.is_open()){
		std::string out{ this->header };
		out.append(this->code);

		outputFile << out;
		outputFile.close();
	}
	else {
		std::cerr << "Unable to open file\n";
	}
	
}



