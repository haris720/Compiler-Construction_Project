#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>

class Lexer {
public:
    Lexer(const std::string& src);
    Token nextToken();

private:
    std::string source;
    size_t pos;

    char peek();
    char get();
    Token makeToken(TokenType type, const std::string& value = "");

    // Helpers
    void skipWhitespace();
    void skipComments(); // skip comments completely
};

#endif

