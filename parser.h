#ifndef PARSE
#define PARSE
#include "lex.h"
#include "emit.h"
#include <unordered_set>

class Parser
{
public:
	lex& lexer;
	Emitter& emitter;
	Token curToken;
	Token peekToken;
	std::unordered_set<std::string> symbols;
	std::unordered_set<std::string> labelsDeclared;
	std::unordered_set<std::string> labelsGotoed;
	//constructor
	Parser(lex&, Emitter&);
	//return true if curr token matches
	bool checkToken(TokenType) const;
	//return true if next token matches
	bool checkPeek(TokenType) const;
	//try to match curr token,if not,error.Andvances the current token
	void match(TokenType);
	// Advances curr token
	void nextToken();
	//Production rules
	//program ::= {statement}
	void program();
	// statment grammar
	void statement();
	// nl::='\n'+ 
	void nl();
	// comparison ::= expression(("=="|"!="|">"|">="|"<"|"<=")expression)+
	void comparison();
	// expression ::= term{('-'|'+')term}
	void expression();
	// return true if curr token is a comparison operator
	bool isComparison();
	// term ::= unary{("/"|"*) unary}
	void term();
	// unary ::= ["+"|"-"] primary
	void unary();
	// primary ::= number | ident
	void primary();
};	

#endif
