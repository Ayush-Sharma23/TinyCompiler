#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "emit.h"
#include "lex.h"
#include "parser.h"

std::ostream& operator<<(std::ostream& os, TokenType type) {
	return os << static_cast<int>(type);
}

int main(int argc, char* argv[]) {
	std::cout << "I'm just a compiler mate :p\n";

	/*
		refactor this into c++:
		if len(sys.argv) != 2:
		sys.exit("Error: Compiler needs source file as argument.")
	*/
	if (argc != 2) {
		abort_("Error : Compiler needs source file as argument\n");
	}
	
	std::ifstream input(argv[1]);

	if (!input.is_open()) {
		std::cerr << "Error: Could not open file.\n";
		return 1;
	}

	std::stringstream buffer;
	buffer << input.rdbuf();
	std::string source{buffer.str()};
	lex lexer(source);
	//Emitter emitter("D:\\cs_prog\\tinyCompiler\\x64\\Debug\\out.c");
	Emitter emitter("out.c");

	Parser parser(lexer,emitter);

	parser.program();
	std::cout << "Parsing Completed.\n";
	emitter.writeFile();
	std::cout << "Compiling Completed.\n";
	return 0;
}