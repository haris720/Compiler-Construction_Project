#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum class TokenType {
    T_INT, T_FLOAT, T_BOOL, T_CHAR, T_STRING,
    T_IF, T_ELSE, T_RETURN, T_WHILE, T_FOR,
    T_IDENTIFIER, T_NUMBER, T_STRING_LITERAL, T_CHAR_LITERAL,
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_ASSIGN, T_EQ, T_NEQ,
    T_LT, T_GT, T_LTE, T_GTE, T_AND, T_OR, T_NOT,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMI, T_COMMA, T_COMMENT, T_EOF, T_UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    std::string toString() const {
        switch (type) {
            case TokenType::T_INT: return "[T_INT(\"" + value + "\")]";
            case TokenType::T_FLOAT: return "[T_FLOAT(\"" + value + "\")]";
            case TokenType::T_BOOL: return "[T_BOOL(\"" + value + "\")]";
            case TokenType::T_CHAR: return "[T_CHAR(\"" + value + "\")]";
            case TokenType::T_STRING: return "[T_STRING(\"" + value + "\")]";
            case TokenType::T_IDENTIFIER: return "[T_IDENTIFIER(\"" + value + "\")]";
            case TokenType::T_NUMBER: return "[T_INTLIT(\"" + value + "\")]";
            case TokenType::T_STRING_LITERAL: return "[T_STRINGLIT(\"" + value + "\")]";
            case TokenType::T_CHAR_LITERAL: return "[T_CHARLIT(\"" + value + "\")]";
            case TokenType::T_COMMENT: return "[T_COMMENT(\"" + value + "\")]";
            case TokenType::T_PLUS: return "[T_OPERATOR(\"+\")]";
            case TokenType::T_MINUS: return "[T_OPERATOR(\"-\")]";
            case TokenType::T_STAR: return "[T_OPERATOR(\"*\")]";
            case TokenType::T_SLASH: return "[T_OPERATOR(\"/\")]";
            case TokenType::T_ASSIGN: return "[T_OPERATOR(\"=\")]";
            case TokenType::T_EQ: return "[T_OPERATOR(\"==\")]";
            case TokenType::T_NEQ: return "[T_OPERATOR(\"!=\")]";
            case TokenType::T_LT: return "[T_OPERATOR(\"<\")]";
            case TokenType::T_GT: return "[T_OPERATOR(\">\")]";
            case TokenType::T_LTE: return "[T_OPERATOR(\"<=\")]";
            case TokenType::T_GTE: return "[T_OPERATOR(\">=\")]";
            case TokenType::T_AND: return "[T_OPERATOR(\"&&\")]";
            case TokenType::T_OR: return "[T_OPERATOR(\"||\")]";
            case TokenType::T_NOT: return "[T_OPERATOR(\"!\")]";
            case TokenType::T_LPAREN: return "[T_LPAREN(\"(\")]";
            case TokenType::T_RPAREN: return "[T_RPAREN(\")\")]";
            case TokenType::T_LBRACE: return "[T_LBRACE(\"{\")]";
            case TokenType::T_RBRACE: return "[T_RBRACE(\"}\")]";
            case TokenType::T_SEMI: return "[T_SEMICOLON(\";\")]";
            case TokenType::T_COMMA: return "[T_COMMA(\",\")]";
            default: return "[T_UNKNOWN(\"" + value + "\")]";
        }
    }
};

#endif
