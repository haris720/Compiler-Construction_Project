#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Define token types
enum class TokenType {
    FUNCTION,
    INT,
    FLOAT,
    STRING,
    RETURN,  // Add this
    IDENTIFIER,
    ASSIGNOP,
    EQUALSOP,
    ADDOP,
    SUBOP,
    MULOP,
    DIVOP,
    COMMA,
    SEMICOLON,
    PARENL,
    PARENR,
    BRACEL,
    BRACER,
    STRINGLIT,
    INTLIT,
    FLOATLIT,
    ERROR
};

// Token structure to store token type and its value
struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

class Lexer {
public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t currentPos;
    char currentChar;

    // Methods to recognize and create tokens
    void advance();
    void skipWhitespace();
    Token consumeIdentifier();
    Token consumeNumber();
    Token consumeStringLiteral();
    Token consumeOperator();
    Token consumeSymbol();

    bool isAlpha(char c) { return std::isalpha(c) || c == '_'; }
    bool isDigit(char c) { return std::isdigit(c); }
};

#endif // LEXER_H