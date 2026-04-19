#include "parser.h"

Parser::Parser(lex& lexer, Emitter& emit): lexer(lexer), emitter(emit) {
	this->lexer = lexer;
	this->emitter = emitter;
	//this->lexer = lexer;
	//this->curToken;
	//this->peekToken;
	this->nextToken();
	this->nextToken();
}

bool Parser::checkToken(TokenType kind) const {
	return kind == curToken.tokenKind;
}

bool Parser::checkPeek(TokenType kind) const {
	return kind == peekToken.tokenKind;
}

void Parser::match(TokenType kind) {
	if (!(this->checkToken(kind))) {
		abort_("Unexpected Token\n");
	}
	this->nextToken();
}

void Parser::nextToken() {
	this->curToken = this->peekToken;
	this->peekToken = this->lexer.getToken();
}

void Parser::program() {
	//std::cout << "PROGRAM\n";
	this->emitter.headerLine("#include <stdio.h>");
	this->emitter.headerLine("int main(){");

	while (this->checkToken(TokenType::NEWLINE)) {
		this->nextToken();
	}

	while (!(this->checkToken(TokenType::EOF_TOKEN))){
		this->statement();
	}

	this->emitter.emitLine("return 0;");
	this->emitter.emitLine("}");

	for (auto label : this->labelsGotoed) {
		if ((this->labelsDeclared.find(label) == this->labelsDeclared.end())) {
			std::string exitCode{ "Attempting to GOTO to undeclared label: " };
			exitCode.append(label);
			abort_(exitCode);
		}
	}

}

void Parser::statement() {
	//check the first token to see what kind of statement 
	// it is
	// "PRINT" (expression | string)
	if (this->checkToken(TokenType::PRINT)) {
		//std::cout << "STATEMENT-PRINT\n";
		this->nextToken();
		std::string out{ "printf(\"" };

		if (this->checkToken(TokenType::STRING)) {
			//simple string
			out.append(this->curToken.text);
			out.append("\\n\");");
			this->emitter.emitLine(out);
			this->nextToken();
		}
		else {
			//expect an expression
			out.push_back('%');
			out.append(".2f\\n\", (float)(");
			this->emitter.emit(out);
			this->expression();
			this->emitter.emitLine("));");

		}
	}
	else if (this->checkToken(TokenType::IF)) {
		//std::cout << "STATEMENT-IF\n";
		this->nextToken();
		this->emitter.emit("if(");
		this->comparison();

		this->match(TokenType::THEN);
		this->nl();
		this->emitter.emitLine("){");

		while (!(this->checkToken(TokenType::ENDIF ))) {
			this->statement();
		}
		this->match(TokenType::ENDIF);
		this->emitter.emitLine("}");
	}
	else if (this->checkToken(TokenType::WHILE)) {
		//std::cout << "STATEMENT-WHILE\n";
		this->nextToken();
		this->emitter.emit("while(");
		this->comparison();

		this->match(TokenType::REPEAT);
		this->nl();
		this->emitter.emitLine("){");

		while (!(this->checkToken(TokenType::ENDWHILE))) {
			this->statement();
		}

		this->match(TokenType::ENDWHILE);
		this->emitter.emitLine("}");
	}
	// LABEL ident
	else if (this->checkToken(TokenType::LABEL)) {
		//std::cout << "STATEMENT-LABEL\n";
		this->nextToken();

		// method 1
		if (this->labelsDeclared.find((this->curToken.text)) != this->labelsDeclared.end()) {
			std::string exitCode("Label already exists: ");
			exitCode.append(this->curToken.text);
			abort_(exitCode);
		}
		this->labelsDeclared.insert(this->curToken.text);

		/*// method 2
		auto result{this->labelsDeclared.insert(this->curToken.text)};
		bool wasInserted{result.second};
		if(!(wasInserted)){
			std::string exitCode("Label already exists: ");
			exitCode.append(this->curToken.text);
			abort_(exitCode);
		*/
		std::string out{ this->curToken.text };
		out.append(":");
		this->emitter.emitLine(out);
		this->match(TokenType::IDENT);
	}
	// GOTO ident
	else if (this->checkToken(TokenType::GOTO)) {
		//std::cout << "STATEMENT-GOTO\n";
		this->nextToken();
		this->labelsGotoed.insert(this->curToken.text);

		std::string out{"goto "};
		out.append(this->curToken.text);
		out.append(";");
		this->emitter.emitLine(out);
		this->match(TokenType::IDENT);
	}
	// LET ident
	else if (this->checkToken(TokenType::LET)) {
		//std::cout << "STATEMENT-LET\n";
		this->nextToken();

		auto p{ this->symbols.insert(this->curToken.text) };
		if (p.second) {
			std::string out{ "float " };
			out.append(this->curToken.text);
			out.append(";");
			this->emitter.headerLine(out);
		}
		std::string out{ this->curToken.text };
		out.append(" = ");
		this->emitter.emit(out);
		this->match(TokenType::IDENT);
		this->match(TokenType::EQ);
		this->expression();
		this->emitter.emitLine(";");
	}
	// INPUT ident
	else if (this->checkToken(TokenType::INPUT)) {
		//std::cout << "STATEMENT-INPUT\n";
		this->nextToken();

		auto p{ this->symbols.insert(this->curToken.text) };
		if (p.second) {
			std::string out{ "float " };
			out.append(this->curToken.text);
			out.append(";");
			this->emitter.headerLine(out);
		}

		//Emit scanf but also validate the input
		//If invalid, set the variable to 0 and clear the input
		std::string o1{ "if(0 == scanf(\"%" };
		o1.append("f\", &");
		o1.append(this->curToken.text);
		o1.append(")) {");
		this->emitter.emitLine(o1);
		std::string o2{this->curToken.text};
		o2.append(" = 0;");
		this->emitter.emitLine(o2);
		this->emitter.emit("scanf(\"%");
		this->emitter.emitLine("*s\");");
		this->emitter.emitLine("}");

		this->match(TokenType::IDENT);
	}
	// INVALID Statement
	else {
		std::string exitCode{ "Invalid statement at" };
		exitCode.append(curToken.text);
		abort_(exitCode);
	}

	this->nl();
}

void Parser::nl() {
	//std::cout << "NEWLINE\n";
	//require at least one newline
	this->match(TokenType::NEWLINE);
	//but allow extra newlines too
	while (this->checkToken(TokenType::NEWLINE)) {
		this->nextToken();
	}
}

void Parser::comparison() {
	//std::cout << "COMPARISON\n";
	this->expression();
	
	//  must be at least 1 comparison op and another exprs
	if (this->isComparison()) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
		this->expression();
	}
	else {
		std::string exitCode{ "Expected Comparison operator at " };
		exitCode.append(this->curToken.text);
		abort_(exitCode);
	}
	// can have 0 or more comparison ops and exprs
	while (this->isComparison()) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
		this->expression();
	}
}

void Parser::expression() {
	//std::cout << "EXPRESSION\n";
	this->term();
	// can have 0 or more +/- and exprs
	while (this->checkToken(TokenType::PLUS) || this->checkToken(TokenType::MINUS)) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
		this->term();
	}
}

bool Parser::isComparison() {

	return (static_cast<int>(this->curToken.tokenKind) >= 206 || static_cast<int>(this->curToken.tokenKind) == 201);
	//return (this->checkToken(TokenType::GT) || this->)
}

void Parser::term() {
	//std::cout << "TERM\n";
	this->unary();

	// can have 0 or more *// expressions
	while (this->checkToken(TokenType::ASTERISK) || this->checkToken(TokenType::SLASH)) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
		this->unary();
	}
}

void Parser::unary() {
	//std::cout << "UNARY\n";

	// optional unary +/-
	if (this->checkToken(TokenType::PLUS) || this->checkToken(TokenType::MINUS)) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
	}
	this->primary();
}

void Parser::primary() {
	//std::cout << "PRIMARY ("<< this->curToken.text <<")\n";

	if (this->checkToken(TokenType::NUMBER)) {
		this->emitter.emit(this->curToken.text);
		this->nextToken();
	}
	else if (this->checkToken(TokenType::IDENT)) {

		if (this->symbols.find(this->curToken.text) == this->symbols.end()) {
			std::string exitCode{ "Referencing variable before assignment: " };
			exitCode.append(this->curToken.text);
			abort_(exitCode);
		}

		this->emitter.emit(this->curToken.text);
		this->nextToken();
	}
	else {
		//ERROR
		std::string exitCode{ "Unexpected Token at " };
		exitCode.append(this->curToken.text);
		abort_(exitCode);
	}
}	