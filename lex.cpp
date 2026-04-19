#include "lex.h"

//start Classifier
bool Classifier::isNum(const char c) {
	if (c >= 48 && c <= 57) {
		return true;
	}
	return false;
}

bool Classifier::isAlpha(const char c) {
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return true;
	}
	return false;
}

bool Classifier::isAlnum(const char c) {
	if (isNum(c) || isAlpha(c)) {
		return true;
	}
	return false;
}
//end Classifier

//start Token
void Token::assignVal(const std::string tokenText,const TokenType tokenKind) {
		this->text = tokenText;
		this->tokenKind = tokenKind;
}

TokenType Token::checkIfKeyword(std::string tokText) {
	static std::unordered_map<std::string, TokenType> keywords = {
		{"LABEL", TokenType::LABEL},
		{"GOTO", TokenType::GOTO},
		{"PRINT", TokenType::PRINT},
		{"INPUT", TokenType::INPUT},
		{"LET", TokenType::LET},
		{"IF", TokenType::IF},
		{"THEN", TokenType::THEN},
		{"ENDIF", TokenType::ENDIF},
		{"WHILE", TokenType::WHILE},
		{"REPEAT", TokenType::REPEAT},
		{"ENDWHILE", TokenType::ENDWHILE}
	};
	if (keywords.count(tokText)) {
		return keywords[tokText];
	}
	return TokenType::IDENT;
}
//end Token

//start lex
lex::lex(std::string source) {
	source.push_back('\n');
	this->source = source;
	this->curChar;
	this->curPos = -1;
	this->nextChar();
}

lex::lex() {}

void lex::nextChar() {
	this->curPos++;
	if (this->curPos >= static_cast<int>( this->source.length()))
		this->curChar = '\0'; //EOF
	else
		this->curChar = this->source[this->curPos];
}

char lex::peek() {
	if (this->curPos + 1 >= static_cast<int>(this->source.length()))
		return '\0';
	return this->source[this->curPos + 1];
}

Token lex::getToken() {
	skipWhitespace();
	skipComment();
	
	Token tk;
	Classifier cl;

	switch (this->curChar) {
	case '+':
		tk.assignVal(std::string(1,this->curChar), TokenType::PLUS);
		break;
	case '-':
		tk.assignVal(std::string(1,this->curChar), TokenType::MINUS);
		break;
	case '*':
		tk.assignVal(std::string(1,this->curChar), TokenType::ASTERISK);
		break;
	case '/':
		tk.assignVal(std::string(1,this->curChar), TokenType::SLASH);
		break;
	case '=':
	{
		if (this->peek()=='=') {
			std::string lastChar{std::string(1, this->curChar ) };
			this->nextChar();
			tk.assignVal(lastChar + std::string(1,this->curChar), TokenType::EQEQ);
		}
		else {
			tk.assignVal(std::string(1,this->curChar), TokenType::EQ);
		}
	}
	break;
	case '!':
	{
		if (this->peek() == '=') {
			std::string lastChar{ std::string(1,this->curChar) };
			this->nextChar();
			tk.assignVal(lastChar + std::string(1,this->curChar), TokenType::NOTEQ);
		}
		else {
			abort_("Expected != , got ! "+ std::to_string(peek()));
		}
	}
	break;
	case '>':
	{
		if (this->peek() == '=') {
			std::string lastChar{ std::string(1,this->curChar) };
			this->nextChar();
			tk.assignVal(lastChar + std::string(1,this->curChar), TokenType::GTEQ);
		}
		else {
			tk.assignVal(std::string(1,this->curChar), TokenType::GT);
		}
	}
	break;
	case '<':
	{
		if (this->peek() == '=') {
			std::string lastChar{ std::string(1,this->curChar) };
			this->nextChar();
			tk.assignVal(lastChar + std::string(1,this->curChar), TokenType::LTEQ);
		}
		else {
			tk.assignVal(std::string(1,this->curChar), TokenType::LT);
		}
	}
	break;
	case '\"':
	{
		this->nextChar();
		int startPos = this->curPos;
		while (this->curChar != '\"') {
			if (this->curChar == '\r' || this->curChar == '\n' || this->curChar == '\\'
				|| this->curChar == '\t' || this->curChar == '%') {
				abort_("Illegal Character In String\n");
			}
			this->nextChar();
		}
		std::string tokText = this->source.substr(startPos, (curPos - startPos));
		tk.assignVal(tokText, TokenType::STRING);
	}
	break;
	case '\n':
		tk.assignVal(std::string(1,this->curChar), TokenType::NEWLINE);
		break;
	case '\0':
		tk.assignVal(std::to_string('\\0'), TokenType::EOF_TOKEN);
		break;
	default:
	{
		if (cl.isNum(this->curChar)) {
			int startPos{ this->curPos };
			while (cl.isNum(this->peek())) {
				this->nextChar();
			}
			if (this->peek() == '.') {
				this->nextChar();
				if (!cl.isNum(this->peek())) {
					abort_("Illegal Character in Number");
				}
				while (cl.isNum(this->peek())) {
					this->nextChar();
				}
			}
			std::string tokText = this->source.substr(startPos, curPos - startPos + 1);
			tk.assignVal(tokText, TokenType::NUMBER);
		}
		else if (cl.isAlpha(this->curChar)) {
			int startPos = this->curPos;
			while (cl.isAlnum(this->peek())) {
				this->nextChar();
			}
			std::string tokText = this->source.substr(startPos, curPos - startPos + 1);
			TokenType kind = tk.checkIfKeyword(tokText);
			tk.assignVal(tokText, kind);
		}
		else {
			abort_("Unexpected Token: " + std::string(1,this->curChar));
		}
	}
		break;
	}
	this->nextChar();
	return tk;
}

void abort_(const std::string errCode) {
	std::cout << errCode << "\n";
	std::exit(0);
}

void lex::skipWhitespace() {
	while (this->curChar == ' ' || this->curChar == '\t' || this->curChar == '\r') {
		this->nextChar();
	}
}

void lex::skipComment() {
	if (this->curChar == '#') {
		while (this->curChar != '\n') {
			this->nextChar();
		}
	}
}
//end lex