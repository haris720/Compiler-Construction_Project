#include "lexer.h"
#include <cctype>
#include <iostream>
#include <unordered_map>

Lexer::Lexer(const std::string& input) : source(input), currentPos(0), currentChar(input[0]) {}

void Lexer::advance() {
    if (currentPos < source.length()) {
        currentPos++;
        if (currentPos < source.length()) {
            currentChar = source[currentPos];
        } else {
            currentChar = '\0';  // End of input
        }
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar)) {
        advance();
    }
}

Token Lexer::consumeIdentifier() {
    std::string identifier;
    while (isAlpha(currentChar) || isDigit(currentChar)) {
        identifier += currentChar;
        advance();
    }

    // Check for keywords
    if (identifier == "fn") return Token(TokenType::FUNCTION, identifier);
    if (identifier == "int") return Token(TokenType::INT, identifier);
    if (identifier == "float") return Token(TokenType::FLOAT, identifier);
    if (identifier == "string") return Token(TokenType::STRING, identifier);
    if (identifier == "return") return Token(TokenType::RETURN, identifier);

    return Token(TokenType::IDENTIFIER, identifier);
}

Token Lexer::consumeNumber() {
    std::string number;
    while (isDigit(currentChar)) {
        number += currentChar;
        advance();
    }

    // Handle floating point numbers
    if (currentChar == '.') {
        number += currentChar;
        advance();
        while (isDigit(currentChar)) {
            number += currentChar;
            advance();
        }
        return Token(TokenType::FLOATLIT, number);
    }

    return Token(TokenType::INTLIT, number);
}

Token Lexer::consumeStringLiteral() {
    std::string str;
    advance();  // Skip opening quote
    while (currentChar != '"' && currentChar != '\0') {
        str += currentChar;
        advance();
    }
    advance();  // Skip closing quote
    return Token(TokenType::STRINGLIT, str);
}

Token Lexer::consumeOperator() {
    if (currentChar == '+') {
        advance();
        return Token(TokenType::ADDOP, "+");
    } else if (currentChar == '-') {
        advance();
        return Token(TokenType::SUBOP, "-");
    } else if (currentChar == '*') {
        advance();
        return Token(TokenType::MULOP, "*");
    } else if (currentChar == '/') {
        advance();
        return Token(TokenType::DIVOP, "/");
    } else if (currentChar == '=') {
        advance();
        // Check for == operator
        if (currentChar == '=') {
            advance();
            return Token(TokenType::EQUALSOP, "==");
        }
        return Token(TokenType::ASSIGNOP, "=");
    }

    return Token(TokenType::ERROR, std::string(1, currentChar));
}

Token Lexer::consumeSymbol() {
    if (currentChar == '(') {
        advance();
        return Token(TokenType::PARENL, "(");
    } else if (currentChar == ')') {
        advance();
        return Token(TokenType::PARENR, ")");
    } else if (currentChar == '{') {
        advance();
        return Token(TokenType::BRACEL, "{");
    } else if (currentChar == '}') {
        advance();
        return Token(TokenType::BRACER, "}");
    } else if (currentChar == ',') {
        advance();
        return Token(TokenType::COMMA, ",");
    } else if (currentChar == ';') {
        advance();
        return Token(TokenType::SEMICOLON, ";");
    }
    return Token(TokenType::ERROR, std::string(1, currentChar));
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (currentChar != '\0') {
        skipWhitespace();
        
        // Check for end of input after skipping whitespace
        if (currentChar == '\0') {
            break;
        }

        if (isAlpha(currentChar)) {
            tokens.push_back(consumeIdentifier());
        }
        else if (isDigit(currentChar)) {
            tokens.push_back(consumeNumber());
        }
        else if (currentChar == '"') {
            tokens.push_back(consumeStringLiteral());
        }
        else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || 
                 currentChar == '/' || currentChar == '=') {  // Add '=' here
            tokens.push_back(consumeOperator());
        }
        else if (currentChar == '(' || currentChar == ')' || currentChar == '{' || currentChar == '}' ||
                 currentChar == ',' || currentChar == ';') {
            tokens.push_back(consumeSymbol());
        }
        else {
            std::cerr << "Invalid token: '" << currentChar << "' (ASCII: " << (int)currentChar << ")" << std::endl;
            advance();
        }
    }

    return tokens;
}