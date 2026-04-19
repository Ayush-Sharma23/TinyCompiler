#ifndef LEX
#define LEX
#include <string>
#include <iostream>
#include <unordered_map>

enum class TokenType {
	EOF_TOKEN = -1,
	NEWLINE = 0,
	NUMBER = 1,
	IDENT = 2,
	STRING = 3,
	// Keywords.
	LABEL = 101,
	GOTO = 102,
	PRINT = 103,
	INPUT = 104,
	LET = 105,
	IF = 106,
	THEN = 107,
	ENDIF = 108,
	WHILE = 109,
	REPEAT = 110,
	ENDWHILE = 111,
	// Operators.
	EQ = 201,
	PLUS = 202,
	MINUS = 203,
	ASTERISK = 204,
	SLASH = 205,
	EQEQ = 206,
	NOTEQ = 207,
	LT = 208,
	LTEQ = 209,
	GT = 210,
	GTEQ = 211
};

class Token
{
public:
	std::string text;
	TokenType tokenKind;
	//Token();
	void assignVal(const std::string,const TokenType);
	TokenType checkIfKeyword(std::string);
};

class lex
{
public:
	std::string source;
	char curChar;
	int curPos;
	lex();
	lex(std::string);
	// process next char
	void nextChar();
	// return the lookahead char
	char peek();
	// invalid token found, abort and throw error
	
	// skip whitespace except newline
	void skipWhitespace();
	// skip comment in code
	void skipComment();
	// return the next token
	Token getToken();
};

class Classifier {
public:
	bool isAlpha(const char);
	bool isNum(const char);
	bool isAlnum(const char);
};

void abort_(const std::string);
#endif