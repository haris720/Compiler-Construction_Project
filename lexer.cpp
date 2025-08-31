#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src), pos(0) {}

char Lexer::peek() {
    return pos < source.size() ? source[pos] : '\0';
}

char Lexer::get() {
    return pos < source.size() ? source[pos++] : '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) get();
}

// Skip both single-line and multi-line comments
void Lexer::skipComments() {
    while (peek() == '/' && pos + 1 < source.size() &&
          (source[pos + 1] == '/' || source[pos + 1] == '*')) {
        if (source[pos + 1] == '/') {
            get(); get(); // skip //
        } else if (source[pos + 1] == '*') {
            get(); get(); // skip /*
        }
        skipWhitespace();
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value};
}
